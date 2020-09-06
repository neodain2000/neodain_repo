#ifndef __NEODAIN_CLASS_IMPL_H__
#define __NEODAIN_CLASS_IMPL_H__

class NeoClassImpl
{
  private:
    int32_t nProp_m;
    NeoClassImpl(const NeoClassImpl& nImpl);
    NeoClassImpl& operator=(const NeoClassImpl& nImpl);

  public:
    // NeoClassImpl() : nProp_m(0) { }
    NeoClassImpl(int32_t n_prop) : nProp_m(n_prop) { }
    // ~NeoClassImpl() { }
    inline int32_t getProperty() { return nProp_m; }
    inline void setProperty(int32_t pt) { nProp_m = pt; }
};

#endif // __NEODAIN_CLASS_IMPL_H__
