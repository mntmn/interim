/* Copyright (C) 2013 by John Cronin <jncronin@tysos.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

 /* This is a file system based upon the raspbootin bootloader by mrvn:
            https://github.com/mrvn/raspbootin

    It is a protocol which supports transfer of a kernel over a serial link.

    It supports two versions of the protocol, the first defined as per the
    above link and a version two which is defined here.

    All commands are initiated by the client, and responses are returned
    by the server.

    Definitions:

    <byte>          A single byte
    <lsb16>         A 16-bit number transmitted in LSB format
    <lsb32>         A 32-bit number transmitted in LSB format
    <msb32>         A 32-bit number transmitted in MSB format
    <string>        A string transmitted in the format:
                        <lsb16 length><byte 0><byte 1>...<byte n>
                    Note not null-terminated and string is in UTF-8 format
    <magic>         The four bytes <byte 0x31><byte 0x41><byte 0x59><byte 0x27>
    <msg_length>    The length of the response sent as a <lsb32> - this is
                        the length minus the length of <magic> and <crc>
    <crc>           A CRC-32 checksum of the command or response transmitted as
                    a <msb32>.  If this fails the command should be re-sent.
                        The particular CRC polynomial is 0x04C11DB7 as per
                        IEEE 802.3
    <dir_entry>     A directory entry in the form:
                        <lsb32 byte_size><lsb32 user_id><lsb32 group_id>
                        <lsb32 properties><string name>
                        properties is a bit-mask of file/directory properties
                        as defined later.

    Commands:

    All commands take the form: <byte \003><byte \003><byte cmd_id><options>

    Compatibility:
        RBTIN_V1            Required for raspbootin v1
        RBTIN_V2_REQ        Required for raspbootin v2
        RBTIN_V2_SPEC       Optional for raspbootin v2 (support is defined
                                in the response to cmd_id 0)

    Error codes:
        0                   Success (returned in error response to cmd_id 1 if
                            there are no entries within the directory)
        -1                  Path not found
        -2                  EOF
        -3                  CRC error in request

    File/directory properties:

    These are defined as per the st_mode field in the man page to lstat(2).
    In particular, S_IFDIR = 0x40000 is set if the entry is a directory.

    Command list:

    cmd_id  Compatibility   Function

    0       RBTIN_V2_REQ    ID and query functionality
                            <options> is <crc>
                            Returns <magic><msg_length><lsb32 error_code>
                            <lsb32 functions><crc> if the server is a valid v2
                            server.
                            Functions is a bit-mask specifying support for each
                            function number (1 to support).  For example, if a
                            server supports functions 0-4 inclusive, this will
                            be 11111b = 0x1f.

    1       RBTIN_V2_SPEC   Read directory entries
                            <options> is <string dir_name><crc>
                            Returns the entries within the specified directory
                            Returns:
                                <magic><msg_length><lsb32 error_code><lsb32 0>
                                    <crc>
                                    - if no entries or error
                                <magic><msg_length><lsb32 0><lsb32 entry_count>
                                    <byte 0><dir_entry 0><dir_entry 1>...
                                    <dir_entry n><crc>
                                    - if success return a list of the entries
                                    - the <byte 0> defines the dir_entry
                                    version and can be changed in future
                                    versions.

    2       RBTIN_V2_SPEC   Read part of a file
                            <options> is <string file_name><lsb32 start>
                            <lsb32 length><crc>
                            Reads the part of a file starting at address start
                            and of length 'length'
                            Returns:
                                <magic><msg_length><lsb32 error_code><lsb32 0>
                                <crc>
                                    - if error
                                <magic><msg_length><lsb32 0><lsb32 bytes_read>
                                <byte 0><byte 1>...<byte n><crc>
                                    - if success

    3       RBTIN_V1        Send the default kernel (all of it)
                            <options> is empty
                            Returns:
                                <lsb32 length><byte 0><byte 1>...<byte n>

    4       RBTIN_V2_SPEC   Display a string on the server console
                            <options> is <string message><crc>
                            Returns:
                                <magic><msg_length><lsb32 error_code><crc>
*/

/* Specifics of the rpi_boot implementation:

    During init, we try and communicate with the server by sending cmd_id 0.
    If a valid response is returned we cache the result and continue.
    If not, we still assume a server is present but that it is v1 (and
    cache supported functionality as 0x8 - only cmd_id 3 supported).

    During directory enumeration/file loading, if a v2 response is cached
    we use that to try and load the file via the v2 functions.

    If v1, and the root directory is enumerated, we return a single member:
        /raspbootin of type file and unknown length
    If v1, and a directory other than the root directory is enumerated, we
        return error code ENOENT
    If v1, and the file /raspbootin is requested, we send cmd_id 3 and save
        the appropriate section to the buffer (and discard the rest)
    If v1, and any other file is requested, we return error code ENOENT
*/

#ifndef ENABLE_SERIAL
#error Raspbootin support requires ENABLE_SERIAL in config.h
#endif

#include <string.h>
#include "fs.h"
#include "output.h"
#include "uart.h"
#include "crc32.h"
#include "block.h"
#include "raspbootin.h"

#define UART_TIMEOUT            10000
#define READDIR_BUF_LEN         0x1000
#define MAX_RETRIES             3

#define CLIENT_CAPABILITIES     0x1f

#define BYTE(num, idx)      (((num) >> ((idx) * 8)) & 0xff)

#ifdef RASPBOOTIN_DEBUG
#define CHECK(resp, floc)   if((resp) == -1) { ret = TIMEOUT; fail_loc = floc; goto cleanup; }
#else
#define CHECK(resp, floc)   if((resp) == -1) { ret = TIMEOUT; goto cleanup; }
#endif

static uint32_t server_capabilities = 9;    // assume server can interpret
                                            // cmds 0 and 3
static uint32_t client_capabilities = CLIENT_CAPABILITIES;

static char raspbootin_name[] = "raspbootin";

static struct dirent *raspbootin_read_directory(struct fs *fs, char **name);
static FILE *raspbootin_fopen(struct fs *fs, struct dirent *path, const char *mode);
static size_t raspbootin_fread(struct fs *fs, void *ptr, size_t size, size_t nmemb, FILE *stream);
static int raspbootin_fclose(struct fs *fs, FILE *fp);
static int send_message(int cmd_id, void *send_buf, size_t send_buf_len,
                        void *recv_buf, size_t recv_buf_len);
static int read_lsb32(uint32_t *val, useconds_t timeout)
{
    uint32_t ret = 0;
    for(int i = 0; i < 4; i++)
    {
        int rbuf = uart_getc_timeout(timeout);
        if(rbuf == -1)
            return -1;
        uint32_t v = (uint32_t)rbuf & 0xff;
        ret |= (v << (i * 8));
    }
    *val = ret;
    return 0;
}

static int send_message_int(int cmd_id, void *send_buf, size_t send_buf_len,
                            void *recv_buf, size_t recv_buf_len)
{
    // Send a message (v2 messages only)
#ifdef RASPBOOTIN_DEBUG
    printf("RASPBOOTIN: sending cmd %i\n");
    int fail_loc = 0;
#endif

    // Check capabilities
    if((cmd_id < 0) || (cmd_id > 31))
    {
        printf("RASPBOOTIN: invalid cmd number %i\n");
        return INVALID_CMD;
    }
    if(cmd_id == 3)
    {
        printf("RASPBOOTIN: cannot use send_message_int to send cmd 3\n");
        return INVALID_CMD;
    }
    uint32_t msg_capabilities = (1 << cmd_id);
    if(!(client_capabilities & msg_capabilities))
    {
        printf("RASPBOOTIN: client does not support cmd %i\n");
        return UNSUPPORTED_CMD;
    }
    if(!(server_capabilities & msg_capabilities))
    {
        printf("RASPBOOTIN: server does not support cmd %i\n");
        return UNSUPPORTED_CMD;
    }

    // The crc of the request is calculated from 'options'
    uint32_t crc = crc32(send_buf, send_buf_len);

    // Disable debug output on the uart
    rpi_boot_output_state ostate = output_get_state();
    output_disable_uart();

    // Clear the receive buffer
    while(uart_getc_timeout(1000) != -1);

    // Send the message
    uart_putc('\003');
    uart_putc('\003');
    uart_putc(cmd_id);

    uint8_t *send_p = (uint8_t *)send_buf;
    while(send_buf_len--)
        uart_putc(*send_p++);

    uart_putc(BYTE(crc, 0));
    uart_putc(BYTE(crc, 1));
    uart_putc(BYTE(crc, 2));
    uart_putc(BYTE(crc, 3));

    // Wait for the response
    usleep(2000);

    // Begin reading response
    int r_buf = 0;
    int ret = 0;

    // Read magic number
    uint32_t magic = 0;
    CHECK(read_lsb32(&magic, UART_TIMEOUT), 0);

    if(magic != MAGIC)
    {
#ifdef RASPBOOTIN_DEBUG
        printf("RASPBOOTIN: invalid magic received: %08x (expecting %08x)\n", magic, MAGIC);
#endif
        ret = INVALID_MAGIC;
        goto cleanup;
    }

    // Read response length
    uint32_t resp_length = 0;
    CHECK(read_lsb32(&resp_length, UART_TIMEOUT), 1);

    // Read error_code
    uint32_t error_code = 0;
    CHECK(read_lsb32(&error_code, UART_TIMEOUT), 2);

    if(error_code != SUCCESS)
    {
        ret = error_code;
#ifdef RASPBOOTIN_DEBUG
        fail_loc = 3;
#endif
        goto cleanup;
    }

    // Read the data (maximum is whatever is greater - recv_buf_len
    // or resp_length)
    // Resp_length is length of the message minus magic and crc
    //  therefore it includes the length of resp_length and error_code
    //  which have already been read, therefore subtract 8
    size_t data_to_read_to_buffer = (size_t)(resp_length - 8);
    size_t data_to_discard = 0;
    size_t data_to_pad = 0;
    if(data_to_read_to_buffer > recv_buf_len)
    {
        data_to_read_to_buffer = recv_buf_len;
        data_to_discard = data_to_read_to_buffer - recv_buf_len;
    }
    else if(recv_buf_len > data_to_read_to_buffer)
        data_to_pad = recv_buf_len - data_to_read_to_buffer;

    crc = crc32_start();
    crc = crc32_append(crc, &magic, 4);
    crc = crc32_append(crc, &resp_length, 4);
    crc = crc32_append(crc, &error_code, 4);
    int data_read = 0;
    uint8_t *rptr = (uint8_t *)recv_buf;
    while(data_to_read_to_buffer--)
    {
        r_buf = uart_getc_timeout(UART_TIMEOUT);
        CHECK(r_buf, 4);
        crc = crc32_append(crc, &r_buf, 1);
        *rptr++ = r_buf;
        data_read++;
    }
    while(data_to_discard--)
    {
        r_buf = uart_getc_timeout(UART_TIMEOUT);
        CHECK(r_buf, 5);
        crc = crc32_append(crc, &r_buf, 1);
    }
    while(data_to_pad--)
        *rptr++ = 0;
    crc = crc32_finish(crc);

    // Read the response CRC
    uint32_t resp_crc = 0;
    CHECK(read_lsb32(&resp_crc, UART_TIMEOUT), 6);
    if(resp_crc != crc)
    {
        ret = CRC_ERROR;
#ifdef RASPBOOTIN_DEBUG
        fail_loc = 7;
#endif
        goto cleanup;
    }

cleanup:
    if(ret == 0)
        ret = data_read;

    // Clear the uart buffer
    while(uart_getc_timeout(1000) != -1);

    // Re-enable uart debug output
    output_restore_state(ostate);

#ifdef RASPBOOTIN_DEBUG
    printf("RASPBOOTIN: send_message_int, returning %i (fail_loc %i)\n",
           ret, fail_loc);
    if(ret == CRC_ERROR)
    {
        printf("RASPBOOTIN: CRC error: read CRC %08x, expected %08x, magic %08x, resp_length %08x, error_code %08x\n",
               resp_crc, crc, magic, resp_length, error_code);
    }
#endif
    return ret;
}

static int send_message(int cmd_id, void *send_buf, size_t send_buf_len,
                        void *recv_buf, size_t recv_buf_len)
{
    // Send a message, retry on CRC failure

    int retries = 0;
    while(retries < MAX_RETRIES)
    {
        int ret = send_message_int(cmd_id, send_buf, send_buf_len,
                                   recv_buf, recv_buf_len);

        if(ret >= 0)
            return ret;
        if(ret != CRC_ERROR)
            return ret;

        retries++;
    }
    return CRC_ERROR;
}

int raspbootin_init(struct fs **fs)
{
    // Try and communicate with a raspbootin server
    uint32_t caps;
    int ret = send_message(0, 0, 0, &caps, sizeof(uint32_t));
    if(ret == sizeof(uint32_t))
    {
        printf("RASPBOOTIN: server contacted.\n");
        server_capabilities = caps;
#ifdef RASPBOOTIN_DEBUG
        printf("RASPBOOTIN: server capabilities: %08x\n", caps);
#endif
    }
    else
    {
#ifdef RASPBOOTIN_DEBUG
        printf("RASPBOOTIN: no server response to cmd 0 (ret %i)\n", ret);
#endif
        server_capabilities = (1 << 3); // only support v1
    }

    // Build a block device structure
    struct block_device *b_dev = (struct block_device *)malloc(sizeof(struct block_device));
    memset(b_dev, 0, sizeof(struct block_device));
    b_dev->device_name = raspbootin_name;

    // Build a fs structure
    struct fs *r_fs = (struct fs *)malloc(sizeof(struct fs));
    memset(r_fs, 0, sizeof(struct fs));
    r_fs->parent = b_dev;
    r_fs->fs_name = raspbootin_name;
    r_fs->fopen = raspbootin_fopen;
    r_fs->fread = raspbootin_fread;
    r_fs->fclose = raspbootin_fclose;
    r_fs->read_directory = raspbootin_read_directory;

    *fs = r_fs;
    return 0;
}

static struct dirent *raspbootin_read_directory(struct fs *fs, char **name)
{
    (void)fs;

    // Join together the directory name
    int dir_name_length = 0;
    char **name_p = name;
    while(*name_p)
    {
        dir_name_length += strlen(*name_p);
        dir_name_length++;
        name_p++;
    }
    dir_name_length++;
    char *dir_name = (char *)malloc(dir_name_length);
    name_p = name;
    char *dir_name_p = dir_name;
    while(*name_p)
    {
        strcpy(dir_name_p, *name_p);
        dir_name_p += strlen(*name_p);
        *dir_name_p++ = '/';
        name_p++;
    }
    *dir_name_p = '\0';

    // Resolve the root directory to an empty string
    int is_root = 0;
    if(strcmp(dir_name, "/"))
    {
        dir_name[0] = '\0';
        is_root = 1;
    }

    // Create a string object of the correct size


    // Send a request for the contents of this directory
    if(server_capabilities & (1 << 1))
    {
        uint8_t *buf = (uint8_t *)malloc(READDIR_BUF_LEN);
        int ret = send_message(1, dir_name, strlen(dir_name),
                               buf, READDIR_BUF_LEN);

        (void)ret;
    }

    (void)is_root;

    return (void *)0;
}

static FILE *raspbootin_fopen(struct fs *fs, struct dirent *path,
                              const char *mode)
{
    (void)fs;
    (void)path;
    (void)mode;
    return (void *)0;
}

static int raspbootin_fclose(struct fs *fs, FILE *fp)
{
    (void)fs;
    (void)fp;
    return 0;
}

static size_t raspbootin_fread(struct fs *fs, void *ptr, size_t size,
                               size_t nmemb, FILE *stream)
{
    (void)fs;
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)stream;
    return 0;
}
