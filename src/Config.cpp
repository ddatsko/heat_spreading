#include <boost/mpi/packed_oarchive.hpp>
#include <boost/mpi/packed_iarchive.hpp>
#include "Config.h"

void Config::serialize(boost::mpi::packed_iarchive & ar, unsigned int version) {
    ar & cycles;
    ar & cycles_per_image;
    ar & delta_t;
    ar & delta_x;
    ar & delta_y;
    ar & alpha;
}

void Config::serialize(boost::mpi::packed_oarchive & ar, unsigned int version) {
    ar & cycles;
    ar & cycles_per_image;
    ar & delta_t;
    ar & delta_x;
    ar & delta_y;
    ar & alpha;
}