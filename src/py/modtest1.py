from augas import *
import log

# stop()

def term(sname):
    log.debug("term(): %s" % sname)

def init(sname):
    log.debug("init(): %s" % sname)
    stop()
