#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/custom.h>
#include <string.h>
#include "libsql.h"

// Custom block operations for database
static void finalize_database(value v) {
    libsql_database_t *db = (libsql_database_t*)Data_custom_val(v);
    if (db) {
        libsql_database_deinit(*db);
    }
}

static struct custom_operations database_ops = {
    "libsql_database",
    finalize_database,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default,
    custom_fixed_length_default
};

// Custom block operations for connection
static void finalize_connection(value v) {
    libsql_connection_t *conn = (libsql_connection_t*)Data_custom_val(v);
    if (conn) {
        libsql_connection_deinit(*conn);
    }
}

static struct custom_operations connection_ops = {
    "libsql_connection",
    finalize_connection,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default,
    custom_fixed_length_default
};

// Helper function to check for errors and raise exception if needed
static void check_error(libsql_error_t *err) {
    if (err != NULL) {
        const char *msg = libsql_error_message(err);
        libsql_error_deinit(err);
        caml_failwith(msg);
    }
}

// Setup libsql with default config
CAMLprim value caml_libsql_setup(value unit) {
    CAMLparam1(unit);
    
    libsql_config_t config = {
        .logger = NULL,
        .version = NULL
    };
    
    const libsql_error_t *err = libsql_setup(config);
    if (err != NULL) {
        const char *msg = libsql_error_message((libsql_error_t*)err);
        caml_failwith(msg);
    }
    
    CAMLreturn(Val_unit);
}

// Initialize database
CAMLprim value caml_libsql_database_init(value path_val) {
    CAMLparam1(path_val);
    CAMLlocal1(result);
    
    const char *path = String_val(path_val);
    
    libsql_database_desc_t desc = {
        .url = NULL,
        .path = path,
        .auth_token = NULL,
        .encryption_key = NULL,
        .sync_interval = 0,
        .cypher = LIBSQL_CYPHER_DEFAULT,
        .disable_read_your_writes = false,
        .webpki = false,
        .synced = false,
        .disable_safety_assert = false
    };
    
    libsql_database_t db = libsql_database_init(desc);
    check_error(db.err);
    
    result = caml_alloc_custom(&database_ops, sizeof(libsql_database_t), 0, 1);
    *((libsql_database_t*)Data_custom_val(result)) = db;
    
    CAMLreturn(result);
}

// Connect to database
CAMLprim value caml_libsql_database_connect(value db_val) {
    CAMLparam1(db_val);
    CAMLlocal1(result);
    
    libsql_database_t *db = (libsql_database_t*)Data_custom_val(db_val);
    libsql_connection_t conn = libsql_database_connect(*db);
    check_error(conn.err);
    
    result = caml_alloc_custom(&connection_ops, sizeof(libsql_connection_t), 0, 1);
    *((libsql_connection_t*)Data_custom_val(result)) = conn;
    
    CAMLreturn(result);
}