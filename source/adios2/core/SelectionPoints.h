/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 */

#ifndef ADIOS2_CORE_SELECTIONPOINTS_H_
#define ADIOS2_CORE_SELECTIONPOINTS_H_

/// \cond EXCLUDE_FROM_DOXYGEN
#include <cstdint>
/// \endcond

#include <vector>

#include "adios2/ADIOSConfig.h"
#include "adios2/core/Selection.h"

namespace adios
{

/** Selection for a selection of an enumeration of positions.
 *  @param     ndim      Number of dimensions
 *  @param     npoints   Number of points of the selection
 *  @param     points    1D array of indices, compacted for all dimension
 *              (e.g.  [i1,j1,k1,i2,j2,k2,...,in,jn,kn] for
 *              n points in a 3D space.
 */
class SelectionPoints : public Selection
{
public:
    SelectionPoints(std::size_t ndim, std::size_t npoints,
                    std::vector<std::uint64_t> &points)
    : Selection(SelectionType::Points), m_Ndim(ndim), m_Npoints(npoints),
      m_Points(points)
    {
    }

    ///< C-style constructor to be used in the C-to-C++ wrapper
    SelectionPoints(std::size_t ndim, std::size_t npoints, uint64_t *points)
    : Selection(SelectionType::Points), m_Ndim(ndim), m_Npoints(npoints),
      m_Points(std::vector<std::uint64_t>()), m_PointsC(points)
    {
    }

    virtual ~SelectionPoints() = default;

    const std::size_t m_Ndim;
    const std::size_t m_Npoints;
    std::vector<std::uint64_t> &m_Points;
    ///< C-to-C++ wrapper needs a pointer to hold the points created by the C
    /// application
    std::uint64_t *m_PointsC = nullptr;
};

} // namespace adios

#endif /* ADIOS2_CORE_SELECTIONPOINTS_H_ */
