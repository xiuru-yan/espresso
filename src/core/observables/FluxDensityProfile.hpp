#ifndef OBSERVABLES_FLUXDENSITYPROFILE_HPP
#define OBSERVABLES_FLUXDENSITYPROFILE_HPP

#include "ProfileObservable.hpp"
#include "particle_data.hpp" 
#include <vector>
#include "integrate.hpp"  


namespace Observables {


class FluxDensityProfile : public ProfileObservable {
public:
    virtual int n_values() const override { return 3 * xbins *ybins*zbins; }
    virtual int actual_calculate() override {
  double ppos[3];
  int img[3];
  if (!sortPartCfg()) {
      runtimeErrorMsg() <<"could not sort partCfg";
    return -1;
  }
  double bin_volume=(maxx-minx)*(maxy-miny)*(maxz-minz)/xbins/ybins/zbins;
    
  for (int id:ids) {
    if (id >= n_part)
      return 1;
/* We use folded coordinates here */
    memmove(ppos, partCfg[id].r.p, 3*sizeof(double));
    memmove(img, partCfg[id].l.i, 3*sizeof(int));
    fold_position(ppos, img);
    int binx= (int) floor( xbins*  (ppos[0]-minx)/(maxx-minx));
    int biny= (int) floor( ybins*  (ppos[1]-miny)/(maxy-miny));
    int binz= (int) floor( zbins*  (ppos[2]-minz)/(maxz-minz));
    if (binx>=0 && binx < xbins && biny>=0 && biny < ybins && binz>=0 && binz < zbins) {
      for(int dim = 0; dim < 3; dim++)
        last_value[3*(binx*ybins*zbins + biny*zbins + binz) + dim] += partCfg[id].m.v[dim]/bin_volume;
    } 
  }
  return 0;
}
};

} // Namespace Observables

#endif

