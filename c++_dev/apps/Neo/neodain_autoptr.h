#ifndef __NEODAIN_AUTOPTR_H__
#define __NEODAIN_AUTOPTR_H__

// #define TEMPLATE_SUPPORTED
#ifdef TEMPLATE_SUPPORTED
#include <memory>
#endif

namespace neodain {
namespace neo {

class NeoClass
{
  private:
    int32_t nprop_m;
  public:
	NeoClass() : nprop_m(1) {}
	~NeoClass() {}
    int32_t getProPerty() { return nprop_m; }
};


#ifdef TEMPLATE_SUPPORTED
typedef std::auto_ptr<NeoClass> NeoClassAutoPtr;
#else
class NeoClassAutoPtr
{
  private:
    NeoClass* pNc_m;
    NeoClassAutoPtr(const NeoClassAutoPtr& ptr);
    NeoClassAutoPtr& operator=(const NeoClassAutoPtr& ptr);
  public:
    explicit NeoClassAutoPtr(NeoClass* pNc = nullptr) : pNc_m(pNc) 
    { 
        std::cout << "class NeoClassAutoPtr is used" << std::endl;
    }
    ~NeoClassAutoPtr() { delete pNc_m; }
    NeoClass& operator*() { return *pNc_m; }
    NeoClass* operator->() {return pNc_m; }
};
#endif


} // namespace neo
} // namespace neodain

#endif
