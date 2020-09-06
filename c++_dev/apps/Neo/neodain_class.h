#ifndef __NEODAIN_NEO_CLASS__
#define __NEODAIN_NEO_CLASS__

#include <assert.h>
#include <memory>

class NeoClassImpl;

class NeoClass 
{
  private:
#if defined(__USE_AUTO_PTR__)
    typedef std::auto_ptr<NeoClassImpl> autoPtr;
    autoPtr ncImpl_m;
#elif defined(__USE_SHARED_PTR__)
    typedef std::shared_ptr<NeoClassImpl> sharedPtr;
    sharedPtr ncImpl_m;
#else
    NeoClassImpl* ncImpl_m;
#endif
  public:
    NeoClass(int32_t pt = 0) : ncImpl_m(new NeoClassImpl(pt)) { }
#if defined(__USE_AUTO_PTR__) || defined(__USE_SHARED_PTR__)
    ~NeoClass() { } 
#else
    ~NeoClass() { delete ncImpl_m; }
#endif
    NeoClass(const NeoClass& nc)
    {
        assert(!"Do not use copy constructor. copy is not allowed for NeoClass");
    }
    NeoClass& operator=(const NeoClass& nc)
    {
        assert(!"Do not use copy assignment. copy is not allowed for NeoClass");
    }
    inline int32_t getProperty() { return ncImpl_m->getProperty(); }
    inline void setProperty(int32_t pt) { ncImpl_m->setProperty(pt); }
};

#endif
