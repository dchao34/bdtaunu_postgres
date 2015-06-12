import sys
import time
import psycopg2
import numpy as np

class SqlDataLoaderError(Exception):

    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)


class SqlDataLoader:
    def __init__(self, database, host,
                 for_update=False,
                 itersize=2000, arraysize=1,
                 rollback=False,
                 debug=False):
        self.conn = psycopg2.connect(host=host, database=database)
        self.select_cursor = None
        self.update_cursor = None
        self.itersize = itersize
        self.arraysize = arraysize
        self.rollback = rollback
        self.for_update = for_update
        self.debug = debug

        return

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        if self.rollback:
            self.conn.rollback()
        else:
            self.conn.commit()
        self.conn.close()
        return

    def deduce_select_stmt(self):

        stmt = 'SELECT eid, b1_mctype, b2_mctype, b1_tau_mctype, b2_tau_mctype FROM McEvent '

        # Debugging
        if self.debug:
            stmt += 'WHERE eid=39938 OR eid=20953473'

        stmt = stmt.strip() + ';'

        return stmt;

    def start(self):
        self.select_cursor = self.conn.cursor('sql_data_reader')
        self.select_cursor.itersize = self.itersize
        self.select_cursor.arraysize = self.arraysize
        self.select_cursor.execute(self.deduce_select_stmt())

        if self.for_update:
            self.update_cursor = self.conn.cursor()

        self.curr_records = self.select_cursor.fetchmany()
        return

    def fetch_next(self):
        self.curr_records = self.select_cursor.fetchmany()
        return

    def update(self, eid_arr, evttype_arr):
        evttype_list, eid_list = evttype_arr.tolist(), eid_arr.tolist()
        update_list = [ (s, r[0]) for s, r in zip(evttype_list, eid_list) ]
        self.update_cursor.executemany(
            'UPDATE McEvent SET mc_evttype = %s WHERE eid = %s;',
            update_list
        )
        return

    def finish(self):
        self.select_cursor.close()
        if self.for_update:
            self.update_cursor.close()
        return


def adapt_records(record_list):

    id_list, num_list = [], []
    for record in record_list:
        id_list.append(int(record[0]))
        num_list.append(map(int, record[1:]))

    id_arr = np.array(id_list).reshape(len(id_list), 1)
    num_arr = np.array(num_list).reshape(len(num_list), 4)

    return id_arr, num_arr


def deduce_target_labels(y_arr):

    b1_mctype, b2_mctype = y_arr[:,0], y_arr[:,1]
    b1_tau_mctype, b2_tau_mctype = y_arr[:,2], y_arr[:,3]

    target_labels = None
    # Declare raw labels
    bmctype_dict = {
        'NoB' : 0,
        'Dtau' : 1,
        'Dstartau' : 2,
        'Dl' : 3,
        'Dstarl' : 4,
        'Dstarstar_res' : 5,
        'Dstarstar_nonres' : 6,
        'SL' : 7,
        'Had' : 8,
        'null' : -1,
    }

    tau_mctype_dict = {
        'NoTau' : 0,
        'tau_e' : 1,
        'tau_mu' : 2,
        'tau_k' : 3,
        'tau_h' : 4,
        'null' : 5,
    }

    # Find records that have the desired characteristics
    dtau = (
        ( b1_mctype == bmctype_dict['Dtau']) |
        ( b2_mctype == bmctype_dict['Dtau'])
    )

    dstartau = (
        ( b1_mctype == bmctype_dict['Dstartau']) |
        ( b2_mctype == bmctype_dict['Dstartau'])
    )

    tau_h = (
        ( b1_tau_mctype == tau_mctype_dict['tau_k'] ) |
        ( b2_tau_mctype == tau_mctype_dict['tau_k'] ) |
        ( b1_tau_mctype == tau_mctype_dict['tau_h'] ) |
        ( b2_tau_mctype == tau_mctype_dict['tau_h'] )
    )

    tau_l = (
        ( b1_tau_mctype == tau_mctype_dict['tau_e'] ) |
        ( b2_tau_mctype == tau_mctype_dict['tau_e'] ) |
        ( b1_tau_mctype == tau_mctype_dict['tau_mu'] ) |
        ( b2_tau_mctype == tau_mctype_dict['tau_mu'] )
    )

    dtau_h = dtau & tau_h
    dstartau_h = dstartau & tau_h
    dtau_l = ( dtau | dstartau ) & tau_l

    sl_ds = (
        ( b1_mctype == bmctype_dict['Dl'] ) |
        ( b2_mctype == bmctype_dict['Dl'] ) |
        ( b1_mctype == bmctype_dict['Dstarl'] ) |
        ( b2_mctype == bmctype_dict['Dstarl'] ) |
        ( b1_mctype == bmctype_dict['SL'] ) |
        ( b2_mctype == bmctype_dict['SL'] )
    )

    sl_dss = (
        ( b1_mctype == bmctype_dict['Dstarstar_res'] ) |
        ( b2_mctype == bmctype_dict['Dstarstar_res'] ) |
        ( b1_mctype == bmctype_dict['Dstarstar_nonres'] ) |
        ( b2_mctype == bmctype_dict['Dstarstar_nonres'] )
    )

    had = (
        ( b1_mctype == bmctype_dict['Had'] ) &
        ( b2_mctype == bmctype_dict['Had'] )
    )

    cont = (
        ( b1_mctype == bmctype_dict['NoB'] ) &
        ( b2_mctype == bmctype_dict['NoB'] )
    )

    # Decide target labels
    target_labels = np.zeros(b1_mctype.shape[0], dtype=np.int32)

    # Declare desired labels
    event_label_dict = {
        'Dtau_h' : 1,
        'Dstartau_h' : 2,
        'sl' : 3,
        'had' : 4,
        'cont' : 5,
    }

    # Dtau_h
    target_labels[dtau_h] = event_label_dict['Dtau_h']
    decided = dtau_h

    # Dstartau_h
    dstartau_h = dstartau_h & ~decided
    target_labels[dstartau_h] = event_label_dict['Dstartau_h']
    decided = dstartau_h | decided

    # Dtau_l
    dtau_l = dtau_l & ~decided
    target_labels[dtau_l] = event_label_dict['sl']
    decided = dtau_l | decided

    # ssl_had
    sl_dss = sl_dss & ~decided
    target_labels[sl_dss] = event_label_dict['sl']
    decided = sl_dss | decided

    # sl_had
    sl_ds = sl_ds & ~decided
    target_labels[sl_ds] = event_label_dict['sl']
    decided = sl_ds | decided

    # had_had
    had = had & ~decided
    target_labels[had] = event_label_dict['had']
    decided = had | decided

    # cont
    cont = cont & ~decided
    target_labels[cont] = event_label_dict['cont']
    decided = cont | decided

    # Sweep
    target_labels[~decided] = event_label_dict['had']

    return target_labels


def print_debug_rows(conn):
    cursor_r2 = conn.cursor()
    cursor_r2.execute('SELECT eid, mc_evttype FROM McEvent WHERE eid=39938 OR eid=20953473;')
    for row in cursor_r2:
        print row


if __name__ == '__main__':

    # 27898336 rows in Event table.
    print 'Started on {0}'.format(time.ctime(time.time()))
    with SqlDataLoader(database='bdtaunu',
                       host='positron02.hep.caltech.edu',
                       itersize=200000,
                       arraysize=200000,
                       for_update=True,
                       rollback=True,
                       debug=True) as sql_loader:

        sql_loader.start()
        while sql_loader.curr_records:
            sys.stdout.write('.')
            sys.stdout.flush()
            eid, bmctype = adapt_records(sql_loader.curr_records)
            mc_evttype = deduce_target_labels(bmctype)
            sql_loader.update(eid, mc_evttype)
            sql_loader.fetch_next()
        sys.stdout.write('Done.\n')

        sql_loader.finish()
        print 'Finished on {0}'.format(time.ctime(time.time()))
        print
