#include "stdafx.h"
#include <vector>
/*#include <algorithm>*/
/*#include <intsafe.h>*/

typedef UINT32 __u32;

extern std::vector<std::shared_ptr<logger_reader>> gveclogger_reader;

/* logger_offset - returns index 'n' into the log via (optimized) modulus */
/*static*/ size_t logger_offset(logger_log *log, size_t n)
{
    return n & (log->size - 1);
}


/*
* get_entry_header - returns a pointer to the logger_entry header within
* 'log' starting at offset 'off'. A temporary logger_entry 'scratch' must
* be provided. Typically the return value will be a pointer within
* 'logger->buf'.  However, a pointer to 'scratch' may be returned if
* the log entry spans the end and beginning of the circular buffer.
*/
logger_entry *get_entry_header(logger_log *log,
    size_t off, logger_entry *scratch)
{
    size_t len = min(LOGGER_ENTRY_STRUCT_SIZE, log->size - off);
    if (len != LOGGER_ENTRY_STRUCT_SIZE) {
        memcpy(((void *) scratch), log->buffer + off, len);
        memcpy(((char *) scratch) + len, log->buffer,
            LOGGER_ENTRY_STRUCT_SIZE - len);
        return scratch;
    }

    return (logger_entry *) (log->buffer + off);
}

/*
* get_entry_msg_len - Grabs the length of the message of the entry
* starting from from 'off'.
*
* An entry length is 2 bytes (16 bits) in host endian order.
* In the log, the length does not include the size of the log entry structure.
* This function returns the size including the log entry structure.
*
* Caller needs to hold log->mutex.
*/
static __u32 get_entry_msg_len(logger_log *log, size_t off)
{
    logger_entry scratch;
    logger_entry *entry;

    entry = get_entry_header(log, off, &scratch);
    return entry->len;
}

/*
* is_between - is a < c < b, accounting for wrapping of a, b, and c
*    positions in the buffer
*
* That is, if a<b, check for c between a and b
* and if a>b, check for c outside (not between) a and b
*
* |------- a xxxxxxxx b --------|
*               c^
*
* |xxxxx b --------- a xxxxxxxxx|
*    c^
*  or                    c^
*/
static inline int is_between(size_t a, size_t b, size_t c)
{
    if (a < b) {
        /* is c between a and b? */
        if (a < c && c <= b)
            return 1;
    } else {
        /* is c outside of b through a? */
        if (c <= b || a < c)
            return 1;
    }

    return 0;
}

/*
* get_next_entry - return the offset of the first valid entry at least 'len'
* bytes after 'off'.
*
* Caller must hold log->mutex.
*/
static size_t get_next_entry(logger_log *log, size_t off, size_t len)
{
    size_t count = 0;

    do {
        size_t nr = LOGGER_ENTRY_STRUCT_SIZE +
            get_entry_msg_len(log, off);
        off = logger_offset(log, off + nr);
        count += nr;
    } while (count < len);

    return off;
}

/*
* fix_up_readers - walk the list of all readers and "fix up" any who were
* lapped by the writer; also do the same for the default "start head".
* We do this by "pulling forward" the readers and start head to the first
* entry after the new write head.
*
* The caller needs to hold log->mutex.
*/
/*static*/ void fix_up_readers(logger_log *log, size_t len)
{
    size_t old = log->w_off;
    size_t new1 = logger_offset(log, old + len);
    struct logger_reader *reader;

    if (is_between(old, new1, log->head))
        log->head = get_next_entry(log, log->head, len);

    for (std::vector<std::shared_ptr<logger_reader>>::iterator it = gveclogger_reader.begin();
        it != gveclogger_reader.end();
        ++ it)
    {
        reader = it[0].get();
        if (is_between(old, new1, reader->r_off))
            reader->r_off = get_next_entry(log, reader->r_off, len);
    }
    /*
    list_for_each_entry(reader, &log->readers, list)
    if (is_between(old, new1, reader->r_off))
    reader->r_off = get_next_entry(log, reader->r_off, len);*/
}

/*
* do_write_log - writes 'len' bytes from 'buf' to 'log'
*
* The caller needs to hold log->mutex.
*/
/*static*/ void do_write_log(logger_log *log, const char *buf, size_t count)
{
    size_t len;

    len = min(count, log->size - log->w_off);
    memcpy(log->buffer + log->w_off, buf, len);
    //!memcpy(log->buffer[log->w_off], buf, len);

    if (count != len)
        memcpy(log->buffer, buf + len, count - len);

    log->w_off = logger_offset(log, log->w_off + count);

}

/*
* do_write_log_user - writes 'len' bytes from the user-space buffer 'buf' to
* the log 'log'
*
* The caller needs to hold log->mutex.
*
* Returns 'count' on success, negative error code on failure.
*/
INT32 do_write_log_from_user(logger_log *log,
    const char *buf, size_t count)
{
    size_t len;

    len = min(count, log->size - log->w_off);
    if (len && !memcpy(log->buffer + log->w_off, buf, len))
        return -1;

    if (count != len)
        if (!memcpy(log->buffer, buf + len, count - len))
            /*
            * Note that by not updating w_off, this abandons the
            * portion of the new entry that *was* successfully
            * copied, just above.  This is intentional to avoid
            * message corruption from missing fragments.
            */
            return -1;

    log->w_off = logger_offset(log, log->w_off + count);

    return count;
}

