#include "DatabaseInstance.h"

MysqlConnection DBInstance::conn = MysqlConnection("tcp://127.0.0.1:3306",
                                                   "ngserver",
                                                   "32z2ksZKUIIyaWdZ",
                                                   "mysql");

//MysqlConnection DBInstance::conn = MysqlConnection("tcp://127.0.0.1:3306",
//                                                   "root",
//                                                   "1234",
//                                                   "mysql");
