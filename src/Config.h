#ifndef MPI_LAB_CONFIG_H
#define MPI_LAB_CONFIG_H

#include <boost/serialization/serialization.hpp>
struct Config {
    friend class boost::serialization::access;

    void serialize(boost::mpi::packed_iarchive & ar, unsigned int version);
    void serialize(boost::mpi::packed_oarchive & ar, unsigned int version);

    int cycles, cycles_per_image;
    double delta_t, delta_x, delta_y;
    double alpha;
};

#endif //MPI_LAB_CONFIG_H
