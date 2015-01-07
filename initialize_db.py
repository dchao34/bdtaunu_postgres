import subprocess
import sys

def createdb(dbname, logfile):
    subprocess.check_call([ "createdb", dbname ], stdout=logfile, stderr=logfile)
    return

def resetdb(dbname, logfile):
    sys.stdout.write("Resetting Database... ")
    sys.stdout.flush()
    subprocess.check_call([ "dropdb", dbname ], stdout=logfile, stderr=logfile)
    createdb(dbname, logfile)
    sys.stdout.write("OK.\n")
    return

def nochangedb(dbname):
    print "Leaving database \"{0}\" unchanged. ".format(dbname)
    return

def createtable(dbname, create_script_name, logfile):
    sys.stdout.write("Creating tables... ")
    sys.stdout.flush()
    subprocess.check_call([ "psql", "-d", dbname,
                            "-a", "-f", create_script_name ],
                          stdout=logfile, stderr=logfile)
    sys.stdout.write("OK.\n")
    return

if __name__ == '__main__':

    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("dbname",
                        help="Database name to connect to.")
    parser.add_argument("--create_script", default="sql/create_tables.sql",
                        help="Path to create table script.")
    parser.add_argument("--log_fname", default="log/initialize_db.log",
                        help="Log file associated to the launching of this script.")
    args = parser.parse_args()

    logfile = open(args.log_fname, 'w')

    try:
        createdb(args.dbname, logfile)
    except subprocess.CalledProcessError:
        while True:
            reset = raw_input("Database already exists. Reset it? y/[n]\n")
            if reset == "y":
                resetdb(args.dbname, logfile)
                break
            elif reset == "n" or reset == '':
                nochangedb(args.dbname)
                sys.exit(0)
            else:
                print
                print "Please enter either \"y\" or \"n\"..."
    try:
        createtable(args.dbname, args.create_script, logfile)
    except subprocess.CalledProcessError:
        pass

    logfile.close()
