//  This file is part of the Icp Library,
//
//  Copyright (C) 2014 by Arnaud TANGUY <arn.tanguy@NOSPAM.gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#ifndef   ERROR_HPP
#define   ERROR_HPP

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace icp
{

/**
 * @brief Abstract interface with all the functions that are needed to compute
 * everything related to the ICP's error
 *
 * \see ErrorPointToPoint, and other error types
 */
template<typename Scalar, unsigned int DegreesOfFreedom, typename PointReference, typename PointCurrent>
class Error {
  public:
    typedef typename pcl::PointCloud<PointReference> Pcs;
    typedef typename pcl::PointCloud<PointCurrent> Pct;
    typedef typename pcl::PointCloud<PointReference>::Ptr PcsPtr;
    typedef typename pcl::PointCloud<PointCurrent>::Ptr PctPtr;
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> VectorX;
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> MatrixX;
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, DegreesOfFreedom> JacobianMatrix;

  protected:
    PctPtr current_;
    PcsPtr reference_;

    //! Vector containing the error for each point
    VectorX errorVector_;
    
    //! Weight matrix to be used in error computation
    //* One weight for each error component */
    MatrixX weights_;

    //! Corresponding Jacobian
    JacobianMatrix J_;

  public:
    /**
     * @brief Computes an error vector from data
     */
    virtual void computeError() = 0;
    /**
     * @brief Computes the Jacobian of the error vector with respect to
     * the optimisation parameters (typically the pose twist)
     */
    virtual void computeJacobian() = 0;

    /**
     * @brief Returns the jacobian matrix. call \c computeJacobian() first.
     *
     * The meaning of the jacobian matrix depends on the wanted error function.
     * See \c ErrorPointToPoint::computeJacobian() for an example 
     *
     * @return The jacobian computed by \c computeJacobian()
     * J_ will be empty in case \c computeJacobian() has never been called, or
     * the data point clouds haven't been set.
     */
    virtual JacobianMatrix getJacobian() const {
      return J_;
    }
    /**
     * @brief Returns the error vector for the specific error function
     *
     * @return the error vector if \c computeError() has been called beforehand
     * and the pointclouds are set, an empty vector otherwise
     */
    virtual VectorX getErrorVector() const {
      return errorVector_;
    }

    /**
     * @brief Provides a pointer the the input target
     *
     * @param model Reference point cloud to be registered against
     */
    virtual void setInputCurrent(const PctPtr &in);
    /**
     * @brief Provides a pointer to the input source (e.g point cloud we want to
     * register)
     *
     * @param[in] Point cloud to be registered 
     */
    virtual void setInputReference(const PcsPtr &in);

    /**
     * @brief Weights every point.
     *
     * \see MEstimator
     *
     * @param w
     * Weight matrix. The meaning of this matrix depends on which error function
     * you use
     */
    virtual void setWeights(const MatrixX& w) {
      weights_ = w;
    } 
};

} /* icp */ 

#endif
