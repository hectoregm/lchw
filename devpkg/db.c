#include <unistd.h>
#include <apr_errno.h>
#include <apr_file_io.h>
#include "db.h"
#include "bstrlib.h"
#include "dbg.h"

static FILE *DB_open(const char *path, const char *mode)
{
        return fopen(path, mode);
}

static void DB_close(FILE *db)
{
        fclose(db);
}

static bstring DB_load(const char *path)
{
        FILE *db = NULL;
        bstring data = NULL;

        db = DB_open(DB_FILE, "r");
        check(db, "Failed to open database: %s", DB_FILE);

        data = bread((bNread)fread, db);
        check(data, "Failed to read from db file: %s", DB_FILE);

        DB_close(db);
        return data;
error:
        if(db) DB_close(db);
        if(data) bdestroy(data);
        return NULL;
}

int DB_update(const char *url)
{
        if(DB_find(url)) {
                log_info("Already recorded as installed: %s", url);
        }

        FILE *db = DB_open(DB_FILE, "a+");
        check(db, "Failed to open DB file: %s", DB_FILE);

        bstring line = bfromcstr(url);
        int rc = bconchar(line, '\n');
        check(rc != BSTR_ERR,
              "Failed to add newline: %s", (char *) line->data);

        rc = fwrite(line->data, blength(line), 1, db);
        check(rc == 1, "Failed to append to the db.");

        DB_close(db);
        return 0;
error:
        if(db) DB_close(db);
        return -1;
}

int DB_find(const char *url)
{
        bstring data = NULL;
        bstring line = bfromcstr(url);
        int res = -1;

        data = DB_load(DB_FILE);
        check(data, "Failed to load: %s", DB_FILE);

        if (binstr(data, 0, line) == BSTR_ERR) {
                res = 0;
        } else {
                res = 1;
        }

error: //fallthrough
        if(data) bdestroy(data);
        if(line) bdestroy(line);

        return res;
}

int BD_init()
{
        apr_pool_t *p = NULL;
        apr_pool_initialize();
        apr_pool_create(&p, NULL);

        int st = access(DB_DIR, W_OK | X_OK);
        check(st == 0, "Failed to obtain access to %s: ", DB_DIR);

        apr_status_t rc = apr_dir_make_recursive(DB_DIR,
                            APR_UREAD | APR_UWRITE | APR_UEXECUTE |
                            APR_GREAD | APR_GWRITE | APR_GEXECUTE, p);
        check(rc == APR_SUCCESS, "Failed to make database dir: %s", DB_DIR);

        st = access(DB_FILE, W_OK);
        check(st == 0, "Failed to obtain access to %s: ", DB_FILE);

        FILE *db = DB_open(DB_FILE, "w");
        check(db, "Cannot open database: %s", DB_FILE);
        DB_close(db);

        apr_pool_destroy(p);
        return 0;
error:
        apr_pool_destroy(p);
        return -1;
}

int DB_list()
{
        bstring data = DB_load(DB_FILE);
        check(data, "Failed to read load: %s", DB_FILE);

        printf("%s", bdata(data));
        bdestroy(data);
        return 0;
error:
        return -1;
}
