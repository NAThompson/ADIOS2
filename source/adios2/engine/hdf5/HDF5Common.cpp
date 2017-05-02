/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * HDF5Common.cpp
 *
 *  Created on: April 20, 2017
 *      Author: Junmin
 */

#include <iostream> //needs to go away, this is just for demo purposes

#include "engine/hdf5/HDF5Common.h"

namespace adios
{

#define H5_ERROR std::cout<<"[ADIOS H5 ERROR] "

HDF5Common::HDF5Common()
  :_writeMode(false),
   _total_timestep(0),
   _currentTimeStep(0)
{
    DefH5T_COMPLEX_FLOAT = H5Tcreate(H5T_COMPOUND, sizeof(std::complex<float>));
    H5Tinsert(DefH5T_COMPLEX_FLOAT, "freal", 0, H5T_NATIVE_FLOAT);
    H5Tinsert(DefH5T_COMPLEX_FLOAT, "fimg", H5Tget_size(H5T_NATIVE_FLOAT),
              H5T_NATIVE_FLOAT);

    DefH5T_COMPLEX_DOUBLE =
        H5Tcreate(H5T_COMPOUND, sizeof(std::complex<double>));
    H5Tinsert(DefH5T_COMPLEX_DOUBLE, "dreal", 0, H5T_NATIVE_DOUBLE);
    H5Tinsert(DefH5T_COMPLEX_DOUBLE, "dimg", H5Tget_size(H5T_NATIVE_DOUBLE),
              H5T_NATIVE_DOUBLE);

    DefH5T_COMPLEX_LongDOUBLE =
        H5Tcreate(H5T_COMPOUND, sizeof(std::complex<long double>));
    H5Tinsert(DefH5T_COMPLEX_LongDOUBLE, "ldouble real", 0, H5T_NATIVE_LDOUBLE);
    H5Tinsert(DefH5T_COMPLEX_LongDOUBLE, "ldouble img",
              H5Tget_size(H5T_NATIVE_LDOUBLE), H5T_NATIVE_LDOUBLE);    
}


void  HDF5Common::H5_Init(const std::string name, MPI_Comm m_MPIComm, bool toWrite)
{
    _writeMode = toWrite;

    //
    _plist_id = H5Pcreate(H5P_FILE_ACCESS);
    
#ifdef ADIOS2_HAVE_MPI
    H5Pset_fapl_mpio(_plist_id, m_MPIComm, MPI_INFO_NULL);
#endif      

    std::string ts0 = "/TimeStep0";

    if (toWrite) {
      /*
       * Create a new file collectively and release property list identifier.
       */
      _file_id = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, _plist_id);
      if (_file_id >= 0) 
      {
	_group_id =
	  H5Gcreate2(_file_id, ts0.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      }
    } else {
      // read a file collectively
      _file_id = H5Fopen(name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
      if (_file_id >= 0) 
      {
	_group_id = H5Gopen(_file_id, ts0.c_str(), H5P_DEFAULT);	
      }
    }

    H5Pclose(_plist_id);
}
	
HDF5Common::~HDF5Common() {}

void HDF5Common::WriteTimeSteps()
{
  if (_file_id < 0) 
  {
    //std::cerr<<"[ADIOS HDF5Error]: Invalid file to record timestep to."<<std::endl;
    H5_ERROR<<"Invalid file to record timestep to."<<std::endl;
    return;
  }

  hid_t s = H5Screate(H5S_SCALAR);

  hid_t attr = H5Acreate(_file_id, "NumTimeSteps", H5T_NATIVE_UINT, s, H5P_DEFAULT, H5P_DEFAULT);
  uint  totalts = _currentTimeStep+1;
  H5Awrite(attr,H5T_NATIVE_UINT,&totalts);

  H5Sclose(s);
  H5Aclose(attr);  
}


int HDF5Common::GetNumTimeSteps()
{
  if (_file_id < 0) 
  {
    std::cerr<<"[ADIOS HDF5Error]: Invalid file to read timestep attribute."<<std::endl;
    return -1;
  }

  if (_total_timestep <= 0) {
    hid_t attr = H5Aopen(_file_id, "NumTimeSteps", H5P_DEFAULT);
    
    H5Aread(attr,H5T_NATIVE_UINT, &_total_timestep);
    H5Aclose(attr);
  }

  return _total_timestep;
}



void HDF5Common::H5_Close() {
    WriteTimeSteps();

    H5Gclose(_group_id);
    H5Fclose(_file_id);
}

void HDF5Common::H5_Advance(int totalts)
{
  _currentTimeStep++;
  if (_currentTimeStep > 0) {
    H5Gclose(_group_id);
  }

  std::string tsname="/TimeStep";
  tsname.append(std::to_string(_currentTimeStep));

  if (_writeMode) {
      _group_id =
	H5Gcreate2(_file_id, tsname.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  } else {    
    if ((totalts > 0) && (totalts <= _currentTimeStep)) 
    {
      return;
    }
    //std::cout<<" ... current  group "<<tsname.c_str()<<std::endl;
    _group_id = 
      H5Gopen(_file_id, tsname.c_str(), H5P_DEFAULT);
  }
}



}
