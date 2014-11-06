#ifndef DmpAlgBgoCutped_H
#define DmpAlgBgoCutped_H

#include "DmpVAlg.h"
#include "DmpEvtBgoPed.h"
#include "DmpEvtBgoRaw.h"
#include "DmpEvtHeader.h"
#include <map>
class DmpEvtHeader;
class DmpEvtBgoRaw;
class DmpEvtBgoPed;

class DmpAlgBgoCutped : public DmpVAlg{
/*
 *  DmpAlgBgoCutped
 *
 */
public:
  DmpAlgBgoCutped();
  ~DmpAlgBgoCutped();

  //void Set(const std::string &type,const std::string &value);
  // if you need to set some options for your algorithm at run time. Overload Set()
  bool Initialize();
  bool ProcessThisEvent();    // only for algorithm
  bool Finalize();
  bool GetPedPar();

private:
  DmpEvtHeader          *fEvtHeader;
  DmpEvtBgoRaw          *fBgoRaw;
  DmpEvtBgoPed          *fBgoPed;
  std::map<short,short>  fGidOrder; // key is global dynode ID

};

#endif
