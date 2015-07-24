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

// Derived from raspbootcom (https://github.com/mrvn/raspbootin)

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#include "crc32.h"
#include "raspbootin.h"
#include "util.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#include <termios.h>
#ifndef UNIX_SOCKETS
#define UNIX_SOCKETS
#endif
#endif // _WIN32

#define READ_BUF_LEN                0x1000

#define SERVER_CAPABILITIES         0x1f

#ifdef _WIN32
typedef HANDLE port_addr;
#else
typedef int port_addr;
#define INVALID_HANDLE_VALUE        -1
#endif

static uint32_t magic = MAGIC;
static uint32_t server_caps = SERVER_CAPABILITIES;

static int do_cmd(int cmd_id, port_addr fd);
static int send_error_msg(port_addr fd, int error_code);

#ifdef UNIX_SOCKETS
static struct sockaddr *sock_addr;
static socklen_t addrlen;
#endif

char *base_dir = NULL;

static inline ssize_t serial_write(port_addr fd, const void *buf, size_t count)
{
#ifdef _WIN32
    ssize_t ret = 0;
    int wf_ret = WriteFile(fd, buf, (DWORD)count, (LPDWORD)&ret, NULL);
    if(wf_ret == 0)
    {
        fprintf(stderr, "WriteFile() failed: %08x\n", GetLastError());
        return 0;
    }
    return ret;
#else
    return write(fd, buf, count);
#endif
}

static inline ssize_t serial_read(port_addr fd, void *buf, size_t count)
{
#ifdef _WIN32
    ssize_t ret = 0;
    int rf_ret = ReadFile(fd, buf, (DWORD)count, (LPDWORD)&ret, NULL);
    if(rf_ret == 0)
    {
        fprintf(stderr, "ReadFile() failed: %08x\n", GetLastError());
        return 0;
    }
    return ret;
#else
    return read(fd, buf, count);
#endif
}

#ifdef UNIX_SOCKETS
static port_addr wait_connection(port_addr fd)
{
    // Check fd type
    struct stat buf;
    if(fstat(fd, &buf) < 0)
    {
        fprintf(stderr, "Error calling fstat(), errno = %i\n", errno);
        return -1;
    }

    if(S_ISSOCK(buf.st_mode))
    {
        // Put in listening mode
        int ret = listen(fd, 1);
        if(ret == -1)
        {
            fprintf(stderr, "Error calling listen(), errno = %i\n", errno);
            return -1;
        }

        // Accept the next connection
        ret = accept(fd, sock_addr, &addrlen);
        if(ret == -1)
        {
            fprintf(stderr, "Error calling accept(), errno = %i\n", errno);
            return -1;
        }

        return fd;
    }
    else
        return fd;
}
#else
#define wait_connection(a) (a)
#endif

static port_addr open_serial(const char *dev)
{
    // Decide on the type of the file
#ifdef UNIX_SOCKETS
    struct stat stat_buf;
    int stat_ret = stat(dev, &stat_buf);

    int is_socket = 0;

    if((stat_ret == -1) && (errno == ENOENT))
        is_socket = 1;
    else if((stat_ret == 0) && S_ISSOCK(stat_buf.st_mode))
        is_socket = 1;

    if(is_socket)
    {
        // Open a unix domain socket
        unlink(dev);
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if(fd == -1)
        {
            fprintf(stderr, "Error creating unix domain socket, errno = %i\n",
                    errno);
            return -1;
        }

        // Prepare the address structure
        struct sockaddr_un *address;
        address = (struct sockaddr_un *)malloc(sizeof(struct sockaddr_un));
        sock_addr = (struct sockaddr *)address;

        memset(address, 0, sizeof(struct sockaddr_un));
        address->sun_family = AF_UNIX;
        strncpy(address->sun_path, dev, sizeof(address->sun_path));

        addrlen = sizeof(struct sockaddr_un);

         // Bind to the socket
        int bind_ret = bind(fd, sock_addr, sizeof(struct sockaddr_un));
        if(bind_ret == -1)
        {
            fprintf(stderr, "Error binding to unix domain socket, errno = %i\n",
                    errno);
            return -1;
        }

        // Wait for a new connection
        return wait_connection(fd);
    }
    else
#endif
    {
#ifdef _WIN32
        port_addr fd = CreateFile(dev, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
        if(fd == INVALID_HANDLE_VALUE)
        {
            fprintf(stderr, "Error opening %s, error: %08x\n", dev, GetLastError());
            return fd;
        }

        DCB config;
        if(GetCommState(fd, &config) == 0)
        {
            fprintf(stderr, "Error calling GetCommState, error %08x\n", GetLastError());
            return INVALID_HANDLE_VALUE;
        }

        // Set the port settings
        config.BaudRate = 115200;
        config.ByteSize = 8;
        config.Parity = NOPARITY;
        config.StopBits = ONESTOPBIT;

        if(SetCommState(fd, &config) == 0)
        {
            fprintf(stderr, "Error calling SetCommState, error %08x\n", GetLastError());
            return INVALID_HANDLE_VALUE;
        }

        // Set the timeouts
        COMMTIMEOUTS timeouts;
        timeouts.ReadIntervalTimeout = 1;
        timeouts.ReadTotalTimeoutMultiplier = 1;
        timeouts.ReadTotalTimeoutConstant = 1;
        timeouts.WriteTotalTimeoutMultiplier = 1;
        timeouts.WriteTotalTimeoutConstant = 1;
        if(SetCommTimeouts(fd, &timeouts) == 0)
        {
            fprintf(stderr, "Error calling SetCommTimeouts, error %08x\n",
                    GetLastError());
            return INVALID_HANDLE_VALUE;
        }

        return fd;
#else
        // The termios structure, to be configured for serial interface
        struct termios termios;

        // Open the device, read only
        int fd = open(dev, O_RDWR | O_NOCTTY);
        if(fd == -1)
        {
            // failed to open
            return -1;
        }

        // Must be a tty to continue set-up, otherwise assume pipe
        if(!isatty(fd))
            return fd;

        // Get the attributes
        if(tcgetattr(fd, &termios) == -1)
        {
            fprintf(stderr, "Failed to get attributes of device %s\n", dev);
            return -1;
        }

        // Poll only
        termios.c_cc[VTIME] = 0;
        termios.c_cc[VMIN] = 0;

        // 8N1 mode
        termios.c_iflag = 0;
        termios.c_oflag = 0;
        termios.c_cflag = CS8 | CREAD | CLOCAL;
        termios.c_lflag = 0;

        // Set 115200 baud
        if((cfsetispeed(&termios, B115200) < 0) ||
        (cfsetospeed(&termios, B115200) < 0))
        {
            fprintf(stderr, "Failed to set baud rate\n");
            return -1;
        }

        // Write the attributes
        if(tcsetattr(fd, TCSAFLUSH, &termios) == -1)
        {
            fprintf(stderr, "Failed to write attributes\n");
            return -1;
        }

        return fd;
#endif
    }
}

int main(int argc, char *argv[])
{
    (void)argc;
    port_addr fd = INVALID_HANDLE_VALUE;

    base_dir = argv[2];

    // Open the serial device
    while(fd == INVALID_HANDLE_VALUE)
    {
        fd = open_serial(argv[1]);

#ifndef _WIN32
        if((fd == INVALID_HANDLE_VALUE) && ((errno == ENOENT) || (errno == ENODEV)))
        {
            fprintf(stderr, "Waiting for %s\n", argv[1]);
            sleep(1);
            continue;
        }
        else
#endif
        if(fd == INVALID_HANDLE_VALUE)
        {
            fprintf(stderr, "Error opening %s\n", argv[1]);
            return -1;
        }
    }

    uint8_t buf;
    int breaks_read = 0;

    for(;;)
    {
        int bytes_read = serial_read(fd, &buf, 1);

        if(bytes_read > 0)
        {
            if(breaks_read == 2)
            {
                int cmd = (int)buf;
                do_cmd(cmd, fd);
                breaks_read = 0;

                // Pause to let the client read the message
                usleep(10000);
            }
            else if(buf == '\003')
                breaks_read++;
            else
            {
                breaks_read = 0;
                putchar(buf);
            }
        }
    }

    return 0;
}

static int do_cmd(int cmd, port_addr fd)
{
    switch(cmd)
    {
        case 0:
        {
            // Identify command

            // Read the command crc
            uint32_t ccrc;
            serial_read(fd, &ccrc, 4);
            uint32_t eccrc = crc32((void *)0, 0);
            if(ccrc != eccrc)
                return send_error_msg(fd, CRC_ERROR);

            // Set the response length and error code
            uint32_t resp_length = 12;
            uint32_t error_code = SUCCESS;

            // Build the response crc
            uint32_t crc = crc32_start();
            crc = crc32_append(crc, &magic, 4);
            crc = crc32_append(crc, &resp_length, 4);
            crc = crc32_append(crc, &error_code, 4);
            crc = crc32_append(crc, &server_caps, 4);
            crc = crc32_finish(crc);

            // Send the response
            serial_write(fd, &magic, 4);
            serial_write(fd, &resp_length, 4);
            serial_write(fd, &error_code, 4);
            serial_write(fd, &server_caps, 4);
            serial_write(fd, &crc, 4);

            fprintf(stderr, "Sent CMD0 response\n");

            return 0;
        }

        case 1:
        {
            // Read directory
            uint32_t eccrc = crc32_start();

            // Read the directory name
            uint16_t dir_name_len;
            serial_read(fd, &dir_name_len, 2);
            eccrc = crc32_append(eccrc, &dir_name_len, 2);
            char *dir_name = (char *)malloc((int)dir_name_len + 1);
            memset(dir_name, 0, dir_name_len + 1);
            serial_read(fd, dir_name, (size_t)dir_name_len);
            eccrc = crc32_append(eccrc, dir_name, (size_t)dir_name_len);
            eccrc = crc32_finish(eccrc);

            // Read the command crc
            uint32_t ccrc;
            serial_read(fd, &ccrc, 4);
            if(ccrc != eccrc)
                return send_error_msg(fd, CRC_ERROR);

            // Append the requested dir to the base dir
            int full_dir_len = strlen(base_dir) + 1 + dir_name_len + 1;
            char *full_dir = (char *)malloc(full_dir_len);
            memset(full_dir, 0, full_dir_len);
            strcat(full_dir, base_dir);
            strcat(full_dir, "/");
            strcat(full_dir, dir_name);

            // Try and read the requested directory
            DIR *dirp = opendir(full_dir);
            if(dirp == NULL)
            {
                free(dir_name);
                free(full_dir);
                if((errno == ENOENT) || (errno == ENOTDIR))
                    return send_error_msg(fd, PATH_NOT_FOUND);
                else
                    return send_error_msg(fd, UNKNOWN_ERROR);
            }

            // Count the directory entries
            int byte_count = 0;
            uint32_t entry_count = 0;
            struct dirent *de;
            while((de = readdir(dirp)) != NULL)
            {
                // Add space for byte_size, user_id, group_id
                //  and props fields
                byte_count += 16;

                // Add space for name string
                byte_count += 2;
                byte_count += strlen(de->d_name);

                entry_count++;
            }
            rewinddir(dirp);

            // Allocate the buffer to send
            uint8_t *buf = (uint8_t *)malloc(byte_count);
            int bptr = 0;

            // Fill in the buffer
            uint32_t entries_filled = 0;
            while((de = readdir(dirp)) != NULL)
            {
                // Build a string of the whole filename
                int fname_len = strlen(de->d_name);
                int path_len = full_dir_len + 1 + fname_len + 1;
                char *path = (char *)malloc(path_len);
                memset(path, 0, path_len);
                strcat(path, full_dir);
                strcat(path, "/");
                strcat(path, de->d_name);

                // Get the file stats
                struct stat stat_buf;
                if(stat(path, &stat_buf) != 0)
                {
                    fprintf(stderr, "Error running fstat on %s, errno = %i\n",
                            path, errno);
                    free(path);
                    free(buf);
                    free(full_dir);
                    free(dir_name);
                    return send_error_msg(fd, UNKNOWN_ERROR);
                }

                // Fill in the buffer
                write_word((uint32_t)stat_buf.st_size, buf, bptr);
                write_word((uint32_t)stat_buf.st_uid, buf, bptr + 4);
                write_word((uint32_t)stat_buf.st_gid, buf, bptr + 8);
                write_word((uint32_t)stat_buf.st_mode, buf, bptr + 12);
                bptr += 16;

                // Fill in the name
                write_halfword((uint16_t)dir_name_len, buf, bptr);
                bptr += 2;
                memcpy(&buf[bptr], de->d_name, dir_name_len);
                bptr += dir_name_len;

                free(path);

                entries_filled++;
            }
            if(entries_filled != entry_count)
            {
                // An error has occurred re-parsing the directory
                fprintf(stderr, "entries_filled (%i) != entry_count (%i)\n",
                        entries_filled, entry_count);
                free(buf);
                free(dir_name);
                free(full_dir);
                send_error_msg(fd, UNKNOWN_ERROR);
            }

            fprintf(stderr, "SERVER: %i directory entries, byte_count %i\n", entry_count, byte_count);

            // Set the response length and error code
            uint32_t resp_length = 16 + byte_count;
            uint32_t error_code = SUCCESS;
            uint32_t dir_entry_version = 0;

            // Build the response crc
            uint32_t crc = crc32_start();
            crc = crc32_append(crc, &magic, 4);
            crc = crc32_append(crc, &resp_length, 4);
            crc = crc32_append(crc, &error_code, 4);
            crc = crc32_append(crc, &entry_count, 4);
            crc = crc32_append(crc, &dir_entry_version, 4);
            crc = crc32_append(crc, buf, byte_count);
            crc = crc32_finish(crc);

            // Send the response
            serial_write(fd, &magic, 4);
            serial_write(fd, &resp_length, 4);
            serial_write(fd, &error_code, 4);
            serial_write(fd, &entry_count, 4);
            serial_write(fd, &dir_entry_version, 4);
            serial_write(fd, buf, byte_count);
            serial_write(fd, &crc, 4);

            fprintf(stderr, "Sent CMD1 response\n");

            free(buf);
            free(dir_name);
            free(full_dir);

            return 0;
        }
    }
    (void)fd;
    return 0;
}

static int send_error_msg(port_addr fd, int error_code)
{
    // Send a fail response - <magic><resp_length = 8><error_code><crc>

    uint32_t resp_length = 8;
    uint32_t crc = crc32_start();
    crc = crc32_append(crc, &magic, 4);
    crc = crc32_append(crc, &resp_length, 4);
    crc = crc32_append(crc, &error_code, 4);
    crc = crc32_finish(crc);

    serial_write(fd, &magic, 4);
    serial_write(fd, &resp_length, 4);
    serial_write(fd, &error_code, 4);
    serial_write(fd, &crc, 4);

    fprintf(stderr, "Sent ERROR %i response\n", error_code);

    return 0;
}
