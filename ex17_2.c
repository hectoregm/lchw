#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
        int id;
        int set;
        char *name;
        char *email;
};

struct Database {
        int MAX_DATA;
        int MAX_ROWS;
        struct Address *rows;
};

struct Connection {
        FILE *file;
        struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
        if (errno) {
                perror(message);
        } else {
                printf("Error %s\n", message);
        }


        if(conn)
                Database_close(conn);

        exit(1);
}

void Address_print(struct Address *addr)
{
        printf("%d %s %s\n",
               addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
        int rc = fread(&conn->db->MAX_ROWS, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to load database: MAX_ROWS", conn);

        rc = fread(&conn->db->MAX_DATA, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to load database: MAX_DATA", conn);


        int max_data = conn->db->MAX_DATA;
        int rows = conn->db->MAX_ROWS;
        int i;
        conn->db->rows = malloc(sizeof(struct Address) * rows);
        for (i = 0; i < rows; ++i) {
                rc = fread(&conn->db->rows[i].id, sizeof(int), 1, conn->file);
                if(rc != 1) die("Failed to load database: Id", conn);
                rc = fread(&conn->db->rows[i].set, sizeof(int), 1, conn->file);
                if(rc != 1) die("Failed to load database: Set", conn);
                conn->db->rows[i].name = malloc(sizeof(char) * max_data);
                rc = fread(conn->db->rows[i].name, sizeof(char) * max_data, 1, conn->file);
                if(rc != 1) die("Failed to load database: Name", conn);
                conn->db->rows[i].email = malloc(sizeof(char) * max_data);
                rc = fread(conn->db->rows[i].email, sizeof(char) * max_data, 1, conn->file);
                if(rc != 1) die("Failed to load database: Email", conn);
        }
}

struct Connection* Database_open(const char *filename, char mode)
{
        struct Connection *conn = malloc(sizeof(struct Connection));
        if(!conn) die("Memory error", conn);

        conn->db = malloc(sizeof(struct Database));
        if(!conn->db) die("Memory error", conn);

        if (mode == 'c') {
                conn->file = fopen(filename, "w");
        } else {
                conn->file = fopen(filename, "r+");

                if (conn->file) {
                        Database_load(conn);
                }
        }

        if (!conn->file) die("Failed to open the file", conn);

        return conn;
}

void Database_close(struct Connection *conn)
{
        if (conn) {
                if(conn->file) fclose(conn->file);
                if(conn->db) {
                        if (conn->db->rows) {
                                struct Address *rows = conn->db->rows;
                                int i;
                                for (i = 0; i < conn->db->MAX_ROWS; ++i) {
                                        if(rows[i].name) free(rows[i].name);
                                        if(rows[i].email) free(rows[i].email);
                                }
                                free(rows);
                        }
                        free(conn->db);
                }
                free(conn);
        }
}

void Database_write(struct Connection *conn)
{
        rewind(conn->file);
        struct Database *db = conn->db;

        int rc = fwrite(&db->MAX_ROWS, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to write database: MAX_ROWS", conn);

        rc = fwrite(&db->MAX_DATA, sizeof(int), 1, conn->file);
        if(rc != 1) die("Failed to write database: MAX_DATA", conn);

        int max_data = db->MAX_DATA;
        int rows = db->MAX_ROWS;
        int i;
        for (i = 0; i < rows; ++i) {
                rc = fwrite(&db->rows[i].id, sizeof(int), 1, conn->file);
                if(rc != 1) die("Failed to write database: Id", conn);
                rc = fwrite(&db->rows[i].set, sizeof(int), 1, conn->file);
                if(rc != 1) die("Failed to write database: Set", conn);
                rc = fwrite(db->rows[i].name, sizeof(char) * max_data, 1, conn->file);
                if(rc != 1) die("Failed to write database: Name", conn);
                rc = fwrite(db->rows[i].email, sizeof(char) * max_data, 1, conn->file);
                if(rc != 1) die("Failed to write database: Email", conn);
        }

        rc = fflush(conn->file);
        if(rc == -1) die("Cannot flush database", conn);
}

void Database_create(struct Connection *conn, int max_rows, int max_data)
{
        int i, j;

        conn->db->MAX_ROWS = max_rows;
        conn->db->MAX_DATA = max_data;

        conn->db->rows = malloc(sizeof(struct Address) * max_rows);

        for (i = 0; i < max_rows; i++) {
                // make a prototype to initialize it
                struct Address addr = {.id = i,.set = 0};
                addr.name = malloc(sizeof(char) * max_data);
                addr.email = malloc(sizeof(char) * max_data);
                for (j = 0; j < max_rows; ++j) {
                        (addr.name)[j] = '\0';
                        (addr.email)[j] = '\0';
                }

                conn->db->rows[i] = addr;
        }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
        struct Address *addr = &conn->db->rows[id];
        if(addr->set) die("Already set, delete it first", conn);

        addr->set = 1;
        // WARNING: bug, read the "How to Break It" and fix this
        char *res = strncpy(addr->name, name, conn->db->MAX_DATA);
        res[conn->db->MAX_DATA - 1] = '\0';
        // demostrate the strncpy bug
        if(!res) die("Name copy failed", conn);

        res = strncpy(addr->email, email, conn->db->MAX_DATA);
        if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
        struct Address *addr = &conn->db->rows[id];
        if(addr->set) {
                Address_print(addr);
        } else {
                die("ID is not set", conn);
        }
}

void Database_delete(struct Connection *conn, int id)
{
        struct Address addr = {.id = id, .set = 0};
        conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
        int i = 0;
        struct Database *db = conn->db;

        for (i = 0; i < db->MAX_ROWS; ++i) {
                struct Address *cur = &db->rows[i];

                if (cur->set) {
                        Address_print(cur);
                }
        }
}

int main(int argc, char *argv[])
{
        if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

        char *filename = argv[1];
        char action = argv[2][0];
        struct Connection *conn = Database_open(filename, action);
        int id = 0;

        if(argc > 3) id = atoi(argv[3]);
        if(action != 'c' && id >= conn->db->MAX_ROWS) die("There's not that many records", conn);

        switch(action) {
        case 'c':
                if(argc != 5) die("Need MAX_ROWS and MAX_DAA", conn);
                int data, rows;
                rows = atoi(argv[3]);
                data = atoi(argv[4]);
                Database_create(conn, rows, data);
                Database_write(conn);
                break;
        case 'g':
                if(argc != 4) die("Need an id to get", conn);

                Database_get(conn, id);
                break;
        case 's':
                if(argc != 6) die("Need id, name, email to set", conn);

                Database_set(conn, id, argv[4], argv[5]);
                Database_write(conn);
                break;
        case 'd':
                if(argc != 4) die("Need id to delete", conn);

                Database_delete(conn, id);
                Database_write(conn);
                break;
        case 'l':
                Database_list(conn);
                break;
        default:
                die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
        }

        Database_close(conn);

        return 0;
}

