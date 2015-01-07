import subprocess
import sys

def get_data_dir(data_label, run):
    if data_label in [ '0', '1' ]:
        return 'pr{0}r{1}'.format(data_label, run)
    else:
        return 'sp{0}r{1}'.format(data_label, run)

def execute_insert(bin_path, ntp_dir, dbname, logfile, data_label, run):
    command = [ args.insert_postgres_path, args.ntp_dir, args.dbname, d, r ]
    subprocess.check_call(command, stderr=logfile)
    return

if __name__ == '__main__':

    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("--insert_postgres_path", default="bin/insert_postgres",
                        help="Path to the insert_postgres executable.")
    parser.add_argument("--ntp_dir", default="ntp",
                        help="Path to the root directory of the ntuples.")
    parser.add_argument("--log_fname", default="log/insert_db.log",
                        help="Log file associated to the launching of this script.")
    parser.add_argument("dbname",
                        help="Database name to connect to.")
    parser.add_argument("data_labels", type=str,
                        help="Comma separated string of data labels to process.")
    parser.add_argument("runs", type=str,
                        help="Comma separated string of runs to process.")
    args = parser.parse_args()

    logfile = open(args.log_fname, 'w')

    print
    print 'Root directory of the input records: {0}'.format(args.ntp_dir)
    print 'Destination database for insertions: {0}'.format(args.dbname)
    print 'Log file output: {0}'.format(args.log_fname)
    print

    data_labels = args.data_labels.split(',')
    runs = args.runs.split(',')
    for d in data_labels:
        for r in runs:
            sys.stdout.write('Inserting records from {0}/{1}/root/*.root'.format(args.ntp_dir, get_data_dir(d, r)))
            sys.stdout.flush()
            try:
                execute_insert(args.insert_postgres_path,
                               args.ntp_dir, args.dbname,
                               logfile,
                               d, r)
                sys.stdout.write(' OK.\n')
                sys.stdout.flush()
            except subprocess.CalledProcessError:
                print
                print ('Error: Problem when inserting {0}. '
                       'See {1} for details.').format(get_data_dir(d, r), args.log_fname)
                sys.exit(1)

    logfile.close()
    print
    print 'Complete! Use \'psql -d {0}\' to see the results.'.format(args.dbname)
