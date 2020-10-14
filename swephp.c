#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include <swe/swephexp.h>

static PHP_FUNCTION(swe_version) {
    char version[10];
    swe_version(version);
    RETURN_STRING(version)
}

static PHP_FUNCTION(swe_set_ephe_path) {
    char spath[255];
    int size;
    struct stat stats;
    stat(spath, &stats);

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &spath, &size) == FAILURE) {
        return;
    }

    if (!S_ISDIR(stats.st_mode)) {
        return;
    }

    swe_set_ephe_path(spath);
}

static PHP_FUNCTION(swe_get_library_path) {
    char spath[255];
    swe_get_library_path(spath);
    RETURN_STRING(spath);
}

static PHP_FUNCTION(swe_calc_ut) {

    double tjd_ut, xx[6];
    int ipl;
    long iflag = SEFLG_SPEED;
    char serr[256];
    long iflgret;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "dl|l", &tjd_ut, &ipl, &iflag) == FAILURE) {
        return;
    }

    iflag |= SEFLG_SPEED;

    iflgret = swe_calc_ut(tjd_ut, ipl, iflag, xx, serr);

    if (iflgret < 0) return;

    array_init(return_value);
    add_next_index_double(return_value, xx[0]);
    add_next_index_double(return_value, xx[1]);
    add_next_index_double(return_value, xx[3]);
}

static PHP_FUNCTION(swe_fixstar_ut) {

    double tjd_ut, xx[6];
    int ipl;
    int size;
    long iflag = SEFLG_SPEED;
    char serr[256];
    long iflgret;
    char star[41];

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sd|l", &star, &size, &tjd_ut, &iflag) == FAILURE) {
        return;
    }

    iflag |= SEFLG_SPEED;

    iflgret = swe_fixstar_ut(star, tjd_ut, iflag, xx, serr);

    if (iflgret < 0) return;

    array_init(return_value);
    add_next_index_double(return_value, xx[0]);
    add_next_index_double(return_value, xx[1]);
}

static PHP_FUNCTION(swe_nod_aps_ut) {

    double tjd_ut, xnasc[6], xndsc[6], xperi[6], xaphe[6];
    int ipl;
    int iflag;
    int method = SE_NODBIT_MEAN;
    char serr[256];
    long iflgret;


    if (zend_parse_parameters(ZEND_NUM_ARGS(), "dl|l", &tjd_ut, &ipl, &iflag) == FAILURE) {
        return;
    }

    iflag |= SEFLG_SPEED;

    iflgret = swe_nod_aps_ut(tjd_ut, ipl, iflag, method, xnasc, xndsc, xperi, xaphe, serr);

    if (iflgret < 0) return;

    RETURN_DOUBLE(xnasc[0])
}


static PHP_FUNCTION(swe_houses) {

    double tjd_ut;
    double geolat;
    double geolon;
    int hsys;
    double cusps[13];
    double ascmc[10];
    long iflgret;


    if (zend_parse_parameters(ZEND_NUM_ARGS(), "dddl", &tjd_ut, &geolat, &geolon, &hsys) == FAILURE) {
        return;
    }

    iflgret = swe_houses(tjd_ut, geolat, geolon, hsys, cusps, ascmc);

    if (iflgret < 0) return;

    array_init(return_value);
    add_next_index_double(return_value, cusps[1]);
    add_next_index_double(return_value, cusps[2]);
    add_next_index_double(return_value, cusps[3]);
    add_next_index_double(return_value, cusps[4]);
    add_next_index_double(return_value, cusps[5]);
    add_next_index_double(return_value, cusps[6]);

    add_next_index_double(return_value, ascmc[0]);
    add_next_index_double(return_value, ascmc[1]);
    add_next_index_double(return_value, ascmc[2]);
    add_next_index_double(return_value, ascmc[3]);
}

static PHP_FUNCTION(swe_houses_armc) {

    double tjd_ut, xx[6];
    double armc;
    double geolat;
    double geolong;
    double eps;
    int hsys;
    double cusps[13];
    double ascmc[10];
    char serr[256];
    long iflgret;

    iflgret = swe_calc_ut(tjd_ut, SE_ECL_NUT, 0, xx, serr);
    eps = xx[0];

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "dddl", &tjd_ut, &geolat, &geolong, &hsys) == FAILURE) {
        return;
    }

    armc = geolong;
    if (armc < 0) armc += 360;

    iflgret = swe_houses_armc(armc, geolat, eps, hsys, cusps, ascmc);
    if (iflgret < 0) return;

    array_init(return_value);
    add_next_index_double(return_value, cusps[1]);
    add_next_index_double(return_value, cusps[2]);
    add_next_index_double(return_value, cusps[3]);
    add_next_index_double(return_value, cusps[4]);
    add_next_index_double(return_value, cusps[5]);
    add_next_index_double(return_value, cusps[6]);

    add_next_index_double(return_value, ascmc[0]);
    add_next_index_double(return_value, ascmc[1]);
    add_next_index_double(return_value, ascmc[2]);
    add_next_index_double(return_value, ascmc[3]);
}

static PHP_FUNCTION(swe_get_planet_name) {
    long ipl;
    char sname[40];

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &ipl) == FAILURE) {
        return;
    }

    swe_get_planet_name(ipl, sname);
    RETURN_STRING(sname);
}

static PHP_FUNCTION(swe_julday) {

    long year;
    long month;
    long day;
    double hour;
    long gregflag;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "llldl", &year, &month, &day, &hour, &gregflag) == FAILURE) {
        return;
    }

    RETURN_DOUBLE(swe_julday(year,month,day,hour, gregflag));
}

static PHP_FUNCTION(swe_date_conversion) {

    long year;
    long month;
    long day;
    double hour;
    char gregflag = 'c';
    double tjd;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "llld", &year, &month, &day, &hour) == FAILURE) {
        return;
    }

    int res = swe_date_conversion(year, month, day, hour, gregflag, &tjd);

    if (res == -1) return; //ERR

    RETURN_DOUBLE(tjd)
}

static PHP_FUNCTION(swe_revjul) {

    double tjd;
    int gregflag = 1;
    int year;
    int month;
    int day;
    double hour;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d|l", &tjd, &gregflag) == FAILURE) {
        return;
    }

    if (gregflag != 0) { gregflag = 1; }

    swe_revjul(tjd, gregflag, &year, &month, &day, &hour);

    array_init(return_value);
    add_assoc_long(return_value, "year", year);
    add_assoc_long(return_value, "month", month);
    add_assoc_long(return_value, "day", day);
    add_assoc_double(return_value, "hour", hour);
}

static PHP_FUNCTION(swe_sidtime) {

    double tjd;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &tjd) == FAILURE) {
        return;
    }

    RETURN_DOUBLE(swe_sidtime(tjd))
}

static PHP_MINIT_FUNCTION(swephp) {
    char version[10];
    swe_set_ephe_path(NULL);
    swe_version(version);

    if (strcmp(version, "2.09.03") != 0) {
        return FAILURE;
    }

    return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(swephp) {
    swe_close();

    return SUCCESS;
}

static zend_function_entry swephp_functions[] = {
    PHP_FE(swe_version, NULL)
    PHP_FE(swe_set_ephe_path, NULL)
    PHP_FE(swe_get_library_path, NULL)
    PHP_FE(swe_calc_ut, NULL)
    PHP_FE(swe_fixstar_ut, NULL)
    PHP_FE(swe_nod_aps_ut, NULL)
    PHP_FE(swe_houses, NULL)
    PHP_FE(swe_houses_armc, NULL)
    PHP_FE(swe_get_planet_name, NULL)
    PHP_FE(swe_julday, NULL)
    PHP_FE(swe_date_conversion, NULL)
    PHP_FE(swe_revjul, NULL)
    PHP_FE(swe_sidtime, NULL)

    PHP_FE_END
};

zend_module_entry swephp_module_entry = {
    STANDARD_MODULE_HEADER,
    "swephp",
    swephp_functions, /* functions */
    PHP_MINIT(swephp),
    PHP_MSHUTDOWN(swephp),
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
    NO_VERSION_YET,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SWEPHP
ZEND_GET_MODULE(swephp)
#endif
