#!/usr/bin/env node

// require('./Main.bs');


// process.stdout.write('Testing');
// console.log(process);

// TODO: add db section to config {type, username, password}
// TODO: remove --mongo flag
// TODO: parse input & check for missing data

const fs = require('fs');
const path = require('path');
const shell = require('shelljs');
const configFile = path.join(__dirname, '..', 'config.json');

if (!fs.existsSync(configFile)) {
    shell.echo(`Error: ${configFile} not found`);

    return;
}

// TODO: make sure server(s) are configured

const config = require(configFile);
const yargs = require('yargs');
const db = yargs.argv.db;
let command = '';

if (yargs.argv.backup) {
    const folder = config.options.folder;
    const server = config.servers.local;

    if (yargs.argv.mongo) {
        const mongo = server.mongo;
        command = `mongodump --db ${db} --out ${folder} -u ${mongo.username} -p ${mongo.password} --authenticationDatabase ${mongo.authDB}`;
    }
} else if (yargs.argv.restore) {
    const folder = config.options.folder;
    const server = config.servers.local;

    if (yargs.argv.mongo) {
        const mongo = server.mongo;
        command = `mongorestore --drop --dir ${folder} --nsInclude ${db}* -u ${mongo.username} -p ${mongo.password} --authenticationDatabase ${mongo.authDB}`;
    }
} else if (yargs.argv.transfer) {
    if (yargs.argv.source === yargs.argv.target) {
        shell.echo('Error: source and target are the same server');

        return;
    } else if (yargs.argv.target !== 'local' && !yargs.argv.skipConfirm) {
        // TODO: get confirmation
    }

    const sourceServer = config.servers[yargs.argv.source];
    const targetServer = config.servers[yargs.argv.target];

    if (yargs.argv.mongo) {
        const sourceMongo = sourceServer.mongo;
        let sourceDump = `mongodump --db ${db} --archive -u ${sourceMongo.username} -p ${sourceMongo.password} --authenticationDatabase ${sourceMongo.authDB}`;
        if (sourceServer.ssh) {
            sourceDump = `ssh ${sourceServer.ssh.username}@${sourceServer.ssh.server} ${sourceDump}`;
        }

        const targetMongo = targetServer.mongo;
        let targetRestore = `mongorestore --archive --drop -u ${targetMongo.username} -p ${targetMongo.password} --authenticationDatabase ${targetMongo.authDB}`;
        if (targetServer.ssh) {
            targetRestore = `ssh ${targetServer.ssh.username}@${targetServer.ssh.server} ${targetRestore}`;
        }

        command = `${sourceDump} | ${targetRestore}`;
    }
    // ssh $1 "mysqldump $4 --add-drop-table $2" | mysql -uroot -proot $3
}

const result = shell.exec(command).code;

if (result !== 0) {
    shell.exit(1);
}