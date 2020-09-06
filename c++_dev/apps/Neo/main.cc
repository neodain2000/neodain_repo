#ifndef __NEODAIN_APP_NEO_MAIN_C__

#ifdef _CFLAG_XX001_
#include "neodain_os.h"
#include "neodain_autoptr.h"

namespace osal = neodain::neo::os;

void f()
{
    int32_t nValue=0;
    neodain::neo::NeoClassAutoPtr testAutoPtr(new neodain::neo::NeoClass());
    nValue = testAutoPtr->getProPerty();
    std::cout << "f() : NeoClass getProPerty() : " << nValue << std::endl;
}

int main()
{
    std::cout << "main start..." << std::endl;
    f();
    return 0;
}

#endif // _CFLAG_XX001_

#ifdef _CFLAG_XX002_
#include "neodain_os.h"

namespace osal = neodain::neo::os;

int main()
{
	std::cout << "pid : " << osal::pid() << std::endl;

	uint64_t check_start_time_1;
	uint64_t check_start_time_2;
	uint64_t check_start_time_3;
	uint64_t check_end_time_1;
	uint64_t check_end_time_2;
	uint64_t check_end_time_3;

	check_start_time_1 = osal::nowtime_t();
	std::cout << "start check time_t  : " << check_start_time_1 << std::endl;
	check_start_time_2 = osal::nowtime_sec();
	std::cout << "start check time_sec : " << check_start_time_2 << std::endl;
	check_start_time_3 = osal::nowtime_msec();
	std::cout << "start check time_msec : " << check_start_time_3 << std::endl;

    char *name = nullptr;
    // name = (char *)malloc(10);
    // memset(name, 0x00, 10);
    // strcpy(name, "abc");
	try {
		neodain::neo::app::MyFunc(name);
	} catch (neodain::neo_ex& e) {
        std::cerr << e.what() << std::endl;
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	} catch (int e) {
		std::cerr << e << std::endl;
	} catch (...) {
		std::cerr << "all exception proceed." << std::endl;
	}
	
	sleep(1);

	check_end_time_1 = osal::nowtime_t();
	std::cout << "end check time_t  : " << check_end_time_1 << std::endl;
	check_end_time_2 = osal::nowtime_sec();
	std::cout << "end check time_sec : " << check_end_time_2 << std::endl;
	check_end_time_3 = osal::nowtime_msec();
	std::cout << "end check time_msec : " << check_end_time_3 << std::endl << std::endl;

	std::cout << "check elapsed time_t : " << (check_end_time_1 - check_start_time_1) << std::endl;
	std::cout << "check elapsed time_sec : " << (check_end_time_2 - check_start_time_2) << std::endl;
	std::cout << "check elapsed time_msec : " << (check_end_time_3 - check_start_time_3) << std::endl;

	return 0;
}	

#endif // _CFLAG_XX002_

#ifdef _CFLAG_XX003_
#include <vector>
#include "neodain_os.h"
#include "neodain_classImpl.h"
#include "neodain_class.h"

#ifdef __USE_SHARED_PTR__
typedef std::shared_ptr<NeoClass> SharedPtr;
typedef std::vector<SharedPtr> SharedPtrVector;
SharedPtr f()
#else
NeoClass* f()
#endif
{
    NeoClass nc(100);
    std::cout << "nc class : first getProperty() : " << nc.getProperty() << std::endl;
#ifdef __USE_SHARED_PTR__
    SharedPtr p_nc(new NeoClass(nc));
#else
    NeoClass* p_nc = new NeoClass(nc);
#endif

#if !defined(__USE_AUTO_PTR__)
    std::cout << "nc class : second getProperty() : " << nc.getProperty() << std::endl;
#endif
    std::cout << "p_nc class : getproperty() : " << p_nc->getProperty() << std::endl;

    return p_nc;
}

#ifdef __USE_SHARED_PTR__
void vf(uint32_t i_max)
{
    uint32_t nMax = i_max;
    SharedPtrVector v_SP;

    v_SP.clear();
    for(uint32_t i=0; i<10; i++)
    {
        if(i==4 && i_max != 10)
        {
            SharedPtr p_nc(nullptr);
            v_SP.push_back(p_nc);
        }
        else
        {
            SharedPtr p_nc(new NeoClass(i));
            v_SP.push_back(p_nc);
        }
    }

    if(v_SP.empty())
    {
        std::cout << "v_SP vector is empty !!!" << std::endl;
    } 
    else
    {
        std::cout << "v_SP vector info : nMax : " << nMax << std::endl;
        std::cout << "v_SP vector info : size : " << v_SP.size() << std::endl;

        if(nMax < v_SP.size()) nMax = v_SP.size() + 1;  
        for(uint32_t n=0; n<nMax; n++)
        {
            if(v_SP.at(n) == nullptr)
            {
                char sbuf[100];
                memset(sbuf, 0x00, sizeof(sbuf));
                sprintf(sbuf, "v_SP.at(%d) is nullptr", n);
                std::cout << "v_SP.at(" << n << ") is nullptr" << std::endl;
                throw neodain::neo_ex(std::string(sbuf), errno);
            }
            else
            {
                std::cout << "v_SP vector class : getproperty() : " << v_SP.at(n)->getProperty() << std::endl;
            }
        }
    }
}
#endif


int main()
{
    std::cout << "shared_ptr test" << std::endl;
    try {
#ifdef __USE_SHARED_PTR__
        SharedPtr m_nc = f();
#else
        NeoClass* m_nc = f();
#endif
        std::cout << "m_nc class : getProperty() : " << m_nc->getProperty() << std::endl;
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
    } catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	} catch (int e) {
		std::cerr << e << std::endl;
	} catch (...) {
		std::cerr << "all exception proceed." << std::endl;
	}

#ifdef __USE_SHARED_PTR__
    try {
        vf(5);
	} catch (neodain::neo_ex& e) {
        std::cout << "called neo_ex exception" << std::endl;
		std::cerr << e.what() << std::endl;
        vf(10);
	} catch (std::runtime_error& e) {
        std::cout << "called runtime_error exception" << std::endl;
		std::cerr << e.what() << std::endl;
    } catch (std::exception& e) {
        std::cout << "called exception" << std::endl;
		std::cerr << e.what() << std::endl;
        vf(10);
	} catch (int e) {
        std::cout << "called int exception" << std::endl;
		std::cerr << e << std::endl;
	} catch (...) {
		std::cerr << "all exception proceed." << std::endl;
	}
#endif
    
    return 0;
}

#endif
#ifdef _CFLAG_XX004_

#include "neodain_os.h"
#include "neodain_common.h"

int main()
{
    std::cout << "neo_snprintf test" << std::endl;

    char sbuf[10];

    memset(sbuf, 0x00, sizeof(sbuf));

    std::cout << "sbuf size : " << sizeof(sbuf) << std::endl;

    // int32_t ns = sprintf(sbuf, "abcd : %d %d", 14, 15); 
    // std::cout << "ns : " << std::string(sbuf) << " size : " << ns << std::endl;
    // memset(sbuf, 0x00, sizeof(sbuf));

    int32_t ns = neo_sprintf(sbuf, sizeof(sbuf), "abcd : %d %d", 14, 15); 

    std::cout << "result : " << std::string(sbuf) << std::endl;
    std::cout << "length : " << ns << std::endl;

    return 0;
}

#endif
#ifdef _CFLAG_XX005_
#include "neodain_os.h"

int main(int argc, char* argv[])
{
    
}

#endif
#ifdef _CFLAG_XX006_
#endif

#endif // __NEODAIN_APP_NEO_MAIN_C__

