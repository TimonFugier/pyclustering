/*!

@authors Andrei Novikov (pyclustering@yandex.ru)
@date 2014-2020
@copyright GNU Public License

@cond GNU_PUBLIC_LICENSE
    pyclustering is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    pyclustering is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
@endcond

*/

#pragma once


#include <memory>

#include <pyclustering/cluster/kmedoids_data.hpp>

#include <pyclustering/utils/metric.hpp>


using namespace pyclustering::utils::metric;


namespace pyclustering {

namespace clst {


/*!

@brief    Defines data representation (point, distance matrix) that is used for processing by K-Medoids algorithm.

*/
enum class kmedoids_data_t {
    POINTS,
    DISTANCE_MATRIX
};


/*!

@brief    Represents K-Medoids clustering algorithm (PAM algorithm) for cluster analysis.
@details  PAM is a partitioning clustering algorithm that uses the medoids instead of centers like in case of K-Means
           algorithm. Medoid is an object with the smallest dissimilarity to all others in the cluster. PAM algorithm
           complexity is \f$O\left ( k\left ( n-k \right )^{2} \right )\f$.

          Implementation based on paper @cite inproceedings::cluster::kmedoids::1.

*/
class kmedoids {
public:
    static const double      DEFAULT_TOLERANCE;     /**< Default value of the tolerance stop condition: if maximum value of change of centers of clusters is less than tolerance then algorithm stops processing. */

    static const std::size_t DEFAULT_ITERMAX;       /**< Default value of the step stop condition - maximum number of iterations that is used for clustering process. */

private:
    static const std::size_t OBJECT_ALREADY_CONTAINED;

    static const std::size_t INVALID_INDEX;

    static const double      NOTHING_TO_SWAP;

private:
    using distance_calculator = std::function<double(const std::size_t, const std::size_t)>;

    struct appropriate_cluster {
    public:
        appropriate_cluster() = default;
        appropriate_cluster(const std::size_t p_index, const double p_distance);

    public:
        std::size_t m_index                 = INVALID_INDEX;
        double      m_distance_to_medoid    = -1.0;
    };

private:
    const dataset                   * m_data_ptr      = nullptr;    /* temporary pointer to input data that is used only during processing */

    kmedoids_data                   * m_result_ptr    = nullptr;    /* temporary pointer to clustering result that is used only during processing */

    medoid_sequence                 m_initial_medoids = { };

    double                          m_tolerance       = DEFAULT_TOLERANCE;

    std::size_t                     m_itermax         = DEFAULT_ITERMAX;

    index_sequence                  m_labels;

    std::vector<double>             m_distance_first_medoid;

    std::vector<double>             m_distance_second_medoid;

    distance_metric<point>          m_metric;

    distance_calculator             m_calculator;

public:
    /*!
    
    @brief    Default constructor of clustering algorithm.
    
    */
    kmedoids() = default;

    /*!
    
    @brief    Constructor of clustering algorithm where algorithm parameters for processing are
               specified.
    
    @param[in] p_initial_medoids: initial medoids that are used for processing.
    @param[in] p_tolerance: stop condition in following way: when maximum value of distance change of
                medoids of clusters is less than tolerance than algorithm will stop processing.
    @param[in] p_itermax: maximum amount of iterations (by default kmedoids::DEFAULT_ITERMAX).
    @param[in] p_metric: distance metric calculator for two points.
    
    */
    kmedoids(const medoid_sequence & p_initial_medoids,
             const double p_tolerance = DEFAULT_TOLERANCE,
             const std::size_t p_itermax = DEFAULT_ITERMAX,
             const distance_metric<point> & p_metric = distance_metric_factory<point>::euclidean_square());

    /*!
    
    @brief    Default destructor of the algorithm.
    
    */
    ~kmedoids();

public:
    /*!
    
    @brief    Performs cluster analysis of an input data.
    
    @param[in]  p_data: input data for cluster analysis.
    @param[out] p_result: clustering result of an input data.
    
    */
    void process(const dataset & p_data, kmedoids_data & p_result);

    /*!
    
    @brief    Performs cluster analysis of an input data.
    
    @param[in]  p_data: input data for cluster analysis.
    @param[in]  p_type: data type (points or distance matrix).
    @param[out] p_result: clustering result of an input data.
    
    */
    void process(const dataset & p_data, const kmedoids_data_t p_type, kmedoids_data & p_result);

private:
    /*!
    
    @brief    Updates clusters in line with current medoids.
    
    */
    double update_clusters();

    /*!
    
    @brief    Creates distance calcultor in line with data type and distance metric metric.
    
    @param[in] p_type: data type (points or distance matrix).
    
    @return   Distance calculator.

    */
    distance_calculator create_distance_calculator(const kmedoids_data_t p_type);

    /*!
    
    @brief    Find appropriate cluster for the particular point.
    
    @param[in] p_index: Index of point that should be placed to cluster.
    @param[in] p_medoids: Medoids that corresponds to clusters.
    
    @return   Index of cluster that is appropriate for the particular point and distance from this point to correspoding medoid. 
               If point is a medoid then OBJECT_ALREADY_CONTAINED value is returned.
    
    */
    appropriate_cluster find_appropriate_cluster(const std::size_t p_index, medoid_sequence & p_medoids);

    /*!
    
    @brief  Swap existed medoid with non-medoid points in order to find the most optimal medoid.

    @return Cost that is needed to swap medoid and non-medoid point.
    
    */
    double swap_medoids();

    /*!
    
    @brief  Calculates cost to swap `p_index_candidate` with the current medoid `p_index_cluster`.

    @param[in] p_index_candidate: index point that is considered as a medoid candidate.
    @param[in] p_index_cluster: index of a cluster where the current medoid is used for calculation.

    @return Cost that is needed to swap medoids.
    
    */
    double calculate_swap_cost(const std::size_t p_index_candidate, const std::size_t p_index_cluster) const;
};


}

}
