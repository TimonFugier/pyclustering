/**
*
* Copyright (C) 2014-2017    Andrei Novikov (pyclustering@yandex.ru)
*
* GNU_PUBLIC_LICENSE
*   pyclustering is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   pyclustering is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/


#pragma once


#include <memory>
#include <vector>


namespace container {

/**
 *
 * @brief   Node of KD Tree.
 *
 */
class kdnode {

friend class kdtree;

public:
  using ptr         = std::shared_ptr<kdnode>;
  using weak_ptr    = std::weak_ptr<kdnode>;

private:
    std::vector<double>           m_data    = { };
    void *                        m_payload = nullptr;

    kdnode::ptr     m_left    = nullptr;
    kdnode::ptr     m_right   = nullptr;
    kdnode::ptr     m_parent  = nullptr;    /* TODO: memory leakage here */
    std::size_t     m_discriminator = 0;

public:
    kdnode(void) = default;

    kdnode(const std::vector<double> & p_data, void * p_payload,  kdnode::ptr p_left, kdnode::ptr p_right, kdnode::ptr p_parent, std::size_t disc);

    kdnode(const kdnode & p_other) = default;

    kdnode(kdnode && p_other) = default;

    virtual ~kdnode(void) = default;

private:
    void set_left(kdnode::ptr p_node);

    void set_right(kdnode::ptr p_node);

    void set_parent(kdnode::ptr p_node);

    void set_discriminator(const std::size_t disc);

public:
    kdnode::ptr get_left(void);

    kdnode::ptr get_right(void);

    kdnode::ptr get_parent(void);

    void * get_payload(void);

    const std::vector<double> & get_data(void) const;

    std::vector<double> & get_data(void);

    double get_value(void) const;

    double get_value(const std::size_t p_discr) const;

    std::size_t get_discriminator(void) const;

    std::size_t get_dimension(void) const;

    void get_children(std::vector<kdnode::ptr> & p_children);
};


bool operator < (const kdnode & node, const std::vector<double> & point);
bool operator < (const std::vector<double> & point, const kdnode & node);

bool operator > (const kdnode & node, const std::vector<double> & point);
bool operator > (const std::vector<double> & point, const kdnode & node);

bool operator <= (const kdnode & node, const std::vector<double> & point);
bool operator <= (const std::vector<double> & point, const kdnode & node);

bool operator >= (const kdnode & node, const std::vector<double> & point);
bool operator >= (const std::vector<double> & point, const kdnode & node);

bool operator == (const kdnode & node, const std::vector<double> & point);
bool operator == (const std::vector<double> & point, const kdnode & node);

}