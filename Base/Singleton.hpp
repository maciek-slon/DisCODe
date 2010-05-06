/*!
 * \file Singleton.hpp
 * \brief File containing the declaration of the Singleton template class.
 *
 * \author tkornuta
 * \date Feb 10, 2010
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <boost/utility.hpp>
#include <boost/thread/once.hpp>
#include <boost/scoped_ptr.hpp>

namespace Base {

/*!
 * \class Singleton
 * \brief Template singleton class, implemented in a Boost-singleton manner.
 * Object is stored in scoped_ptr and instance initialization is protected by Boost::call_once.
 * Warning: If T's constructor throws, instance() will return a null reference.
 * \author tkornuta
 */
template<class T>
class Singleton : private boost::noncopyable
{
private:
     static boost::scoped_ptr<T> t;
     static boost::once_flag flag;

protected:
     Singleton() {}

     virtual ~Singleton() {}

public:
     /*!
      * Method returns a non-copyable reference to stored object instance.
      */
    static T& instance()
    {
    	// Initialize object only once.
        boost::call_once(init, flag);
        // Return reference to object.
        return *t;
    }

    /*!
     * Method initializates the object and calls it's constructor.
     */
    static void init()
    {
    	// Call constructor of the T object.
        t.reset(new T());
    }

};

}//: namespace Base

/*!
 * Initialization of scoped pointer.
 */
template<class T> boost::scoped_ptr<T> Base::Singleton<T>::t(0);

/*!
 * Initialization of once_flag.
 */
template<class T> boost::once_flag Base::Singleton<T>::flag = BOOST_ONCE_INIT;




#endif /* SINGLETON_HPP_ */
