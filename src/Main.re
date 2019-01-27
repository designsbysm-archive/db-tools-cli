open Belt.Result;

type action =
  | Empty
  | Backup
  | Restore
  | Transfer;

type args = {
  mutable action,
  mutable db: string,
  mutable source: string,
  mutable target: string,
};

/* TODO: move to it's own module */
/* TODO: replace with polymorphic variant */
/* TODO: add all codes */
type logColor =
  /* | Dim */
  | Red
  | Yellow
  | White;

let logInfo = (command, message, color) => {
  let colorCode =
    switch (color) {
    | Red => "\027[31m"
    | Yellow => "\027[33m"
    | White => "\027[37m"
    /* | Dim => "\027[2m" */
    };

  /* IDEA: turn into list list of color codes and text */
  /* IDEA: recurively turn into string */

  /* let messageList = [
       "\x1b[2m", /* dim */
       "\x1b[37m", /* white */
       {j|[$command] |j},
       "\x1b[0m", /* reset */
       "\x1b[1m", /* bold */
       colorCode,
       message,
       "\x1b[0m" /* reset */
     ]; */

  /* Js.log(messageList); */

  Js.log(
    {j|\x1b[2m\x1b[37m[$command]\x1b[0m \x1b[1m$colorCode$message\x1b[0m|j},
  );
};
let verifyArgs = _ => {
  let args = {action: Empty, db: "", source: "", target: ""};
  let speclist = [
    (
      "--backup",
      Arg.Unit(
        _ => {
          args.action = Backup;
          args.source = "localhost";
        },
      ),
      " backup local database to the <folder> in config.json",
    ),
    (
      "--restore",
      Arg.Unit(
        _ => {
          args.action = Restore;
          args.target = "localhost";
        },
      ),
      " restore local database from the <folder> in config.json",
    ),
    (
      "--transfer",
      Arg.Unit(_ => args.action = Transfer),
      " transfer database from the --source <server> to the --target <server>",
    ),
    (
      "--db",
      Arg.String(value => args.db = value),
      " <name> of database found in config.json",
    ),
    (
      "--source",
      Arg.String(value => args.source = value),
      " source <server> found in config.json",
    ),
    (
      "--target",
      Arg.String(value => args.target = value),
      " target <server> found in config.json",
    ),
    ("-help", Arg.Unit(_ => ()), ""),
  ];

  /* IDEA: if no db & mongo, execute "all" database */

  let alignedSpeclist = Arg.align(speclist);
  /* TODO: use Arg.parse_argv_dynamic to update speclist based on action */
  /* TODO: update Arg.parse_argv usage message */
  Arg.parse(alignedSpeclist, _ => (), "dbTools Options:");
  let {action, db, source, target} = args;

  /* TODO: create function to generate speclist string */
  switch (action, db, source, target) {
  | (Empty, _, _, _) => Error(`missing_action)
  | (_, db, _, _) when db == "" =>
    Error(
      `missing_option(
        Arg.usage_string(
          alignedSpeclist,
          "Required command option(s) missing:",
        ),
      ),
    )
  | (Backup, _, source, _) when source == "" =>
    Error(
      `missing_option(
        Arg.usage_string(
          alignedSpeclist,
          "Required command option(s) missing:",
        ),
      ),
    )
  | (Restore, _, _, target) when target == "" =>
    Error(
      `missing_option(
        Arg.usage_string(
          alignedSpeclist,
          "Required command option(s) missing:",
        ),
      ),
    )
  | (Transfer, _, source, target) when source == "" || target == "" =>
    Error(
      `missing_option(
        Arg.usage_string(
          alignedSpeclist,
          "Required command option(s) missing:",
        ),
      ),
    )
  | (Transfer, _, source, target) when source == target =>
    Error(`same_servers)
  | (_, _, _, _) => Ok(`success_args(args))
  };
};

let verifyConfig = result => {
  /* Js.log(Node.Fs.existsSync("./config.json")); */
  /* let config =
     if (Node.Fs.existsSync("./config.json")) {
     let json = Node.Fs.readFileSync("./config.json", `utf8);
     /* TODO: decode json */
     /* TODO: create new command with config/servers */

     /* IDEA: require JSON instead of read */
     /* IDEA: use Dict or Dirive abstract instead of bs-json */

     command;
     } else {
     Error("config.json not found");
     };
     }; */
  switch (result) {
  | Error(_) => result
  | Ok(_) =>
    logInfo("verifyConfig", "load config & add servers to command", White);

    Ok(`success_command("command"));
  };
};

let executeCommand = result => {
  /* let exec =
     switch (command) {
     | Backup(db, source) => "mongodump --db ${name} --out ${folder} -u ${mongo.username} -p ${mongo.password} --authenticationDatabase ${mongo.authDB}"
     | Restore(db, target) => "mongorestore --archive --drop -u ${targetMongo.username} -p ${targetMongo.password} --authenticationDatabase ${targetMongo.authDB}"
     | Transfer(db, source, target) => "<ssh server> mongodump ... | <ssh server> mongorestore ..."
     | _ => "unknown command"
     }; */
  switch (result) {
  | Error(_) => result
  | Ok(_) =>
    logInfo(
      "executeCommand",
      "exec command and get success/error message (exit code & stderr?)",
      White,
    );

    Ok(`success_result("message"));
  };
};
let displayResult = result => {
  let (message, color) =
    switch (result) {
    | Error(code) =>
      let message =
        switch (code) {
        | `missing_action => "Missing action option [--backup | --restore | --transfer] is required."
        | `missing_option(specString) => specString
        | `same_servers => "--source and --target can't point to the same server"
        | _ => "Unknown error"
        };
      (message, Red);

    | Ok(code) =>
      switch (code) {
      | `success_result(_) => ("Success", Yellow)
      | _ => ("Unknown result", Red)
      }
    };

  logInfo("dbTools", message, color);
  Js.log("");
};

Error(`empty) |> verifyArgs |> verifyConfig |> executeCommand |> displayResult;