
#include<list>
#include<memory>

namespace xlw
{
    namespace impl
    { 
        namespace details
        {

            // the control block Hierarchy 
            class  control_block
            {
                //friend struct control_block_policy;
            public :
                long use_count()const{return count;}// Never throws 
                void * get()const{return p;}// Never throws 

                virtual void release ()throw()=0; // CANNOT throw
                virtual control_block * increment()throw() // CANNOT throw
                {
                    ++count;
                    return this;
                }
                virtual long decrement() throw() // CANNOT throw
                {
                    if (count==1)
                    {    
                        release();
                    }
                    return --count;
                }

                virtual ~control_block(){};
            protected :
                control_block(void * p_):p(p_), count(1){}
                
                void *p;
                long count;
            };

            class  null_ptr :public control_block
            {
            public:
                null_ptr(): control_block(0){}
                virtual void release ()throw(){}

            };


            template<class T>
            class default_control_block :public control_block
            {
                public:
                default_control_block(T * p):control_block(p){}
                void release ()throw() // CANNOT throw
                {
                    delete static_cast<T*>(p);
                }
            };
            template<class T, class D>
            class destroyer_control_block :public control_block
            {
                public:
                // the copy constructor of D is not allowed to throw
                destroyer_control_block(T* p, D d_):control_block(p),d(d_){}
                void release ()throw() // CANNOT throw
                {
                    (static_cast<T*>(p));
                }
                D d;

            };


            struct control_block_policy
            {
                typedef control_block * control;

                template<class T>
                // may throw BUT will clean up p too if it does 
                static control controlled_alloc( T * p)
                {
                    std::auto_ptr<T> temp_holder(p);

                    // if the following line throws we clean up p too
                    control_block *cb_ptr(new default_control_block<T>(p)) ;
                    temp_holder.release();
                    return cb_ptr;

                }

                template<class T>
                // may throw BUT will clean up p too if it does 
                static control controlled_alloc(std::auto_ptr<T> &r)
                {

                    // if the following line throws we ensue
                    // r still owns p
                    control_block *cb_ptr(new default_control_block<T>(r.get())) ;
                    r.release();
                    return cb_ptr;

                }

                static control controlled_alloc()
                {

                    return new null_ptr();
                }

                template<class T,class D>
                // may throw BUT will clean up p too if it does
                // uses d to clean p with d(p). d(p) CANNOT throw
                static control controlled_alloc( T * p, D d)
                {
                    control cb_ptr = 0;
                    try
                    {

                        // if the following line throws we clean up p too
                        cb_ptr =  new default_control_block<T>(p) ;
                        return cb_ptr;

                    }
                    catch(...)
                    {
                        d(p);
                        throw;
                    }
                }

                static void decrement( control p)throw() // CANNOT throw;
                {
                    if(p)
                    {
                        if(!p->decrement())
                        {
                            delete p;
                        }
                    }
                }

                static control_block * increment(control p)throw() // CANNOT throw;
                {
                    return p->increment();
                }

                static void *get(control p)
                {
                    return p->get();
                }

                static long use_count(control p)
                {
                    return p->use_count();
                }

                static void swap(control &p, control &q)
                {
                    std::swap(p,q);
                }

            };

        }
    }
}