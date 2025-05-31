(* Type definitions *)
type database
type connection

(* External function declarations *)
external setup : unit -> unit = "caml_libsql_setup"
external database_init : string -> database = "caml_libsql_database_init"
external database_connect : database -> connection = "caml_libsql_database_connect"

(* High-level functions *)
let open_database path =
  setup ();
  let db = database_init path in
  database_connect db

let open_memory_database () =
  open_database ":memory:"