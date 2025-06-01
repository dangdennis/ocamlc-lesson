open Libsql

let () =
  try
    Printf.printf "Testing libsql bindings...\n";

    (* Test opening an in-memory database *)
    Printf.printf "Opening in-memory database...\n";
    let _ = open_memory_database () in
    Printf.printf "Success! Database connection established.\n";

    (* Test opening a file database *)
    Printf.printf "Opening file database...\n";
    let _ = open_database "test.db" in
    Printf.printf "Success! File database connection established.\n";

    Printf.printf "All tests passed!\n"
  with
  | Failure msg ->
      Printf.printf "Error: %s\n" msg;
      exit 1
  | e ->
      Printf.printf "Unexpected error: %s\n" (Printexc.to_string e);
      exit 1