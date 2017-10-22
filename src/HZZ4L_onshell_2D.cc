#include "Riostream.h" 
#include "../interface/HZZ4L_onshell_2D.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h>
#include "TMath.h"
#include "TH3F.h"
#include "TAxis.h"
#include "RooDataHist.h"
#include "RooExtendPdf.h"
#include "RooProdPdf.h"

using namespace TMath;

ClassImp(HZZ4L_onshell_2D) 

	HZZ4L_onshell_2D::HZZ4L_onshell_2D(const char *name, const char *title, 
			RooAbsReal& _mass,
			RooAbsReal& _dbkg,
			const RooArgList& inFuncList, 
			const RooArgList& inCoefList): 
		RooAbsPdf(name,title), 
		mass("mass","mass",this,_mass),
		dbkg("dbkg","dbkg",this,_dbkg),
		_FuncList("FuncList","list of FuncList",this),
		_coefList("coefList","List of funcficients",this) 

{ 
	TIterator* FuncIter = inFuncList.createIterator() ;
	RooAbsArg* func;
	while((func = (RooAbsArg*)FuncIter->Next())) {
		if (!dynamic_cast<RooAbsReal*>(func)) {
			assert(0);
		}
		_FuncList.add(*func) ;
	}
	TIterator* coefIter = inCoefList.createIterator() ;
	while((func = (RooAbsArg*)coefIter->Next())) {
		if (!dynamic_cast<RooAbsReal*>(func)) {
			assert(0);
		}
		_coefList.add(*func) ;
	}
	delete FuncIter;
	delete coefIter;

} 


HZZ4L_onshell_2D::HZZ4L_onshell_2D(const HZZ4L_onshell_2D& other, const char* name) :  
	RooAbsPdf(other,name), 
	mass("mass",this,other.mass),
	dbkg("dbkg",this,other.dbkg),
	_FuncList("FuncList",this,other._FuncList),
	_coefList("coefList",this,other._coefList)

{ 
	//_coefIter = _coefList.createIterator() ;
	//_FuncIter = _FuncList.createIterator() ;
} 


Double_t HZZ4L_onshell_2D::evaluate() const 
{ 
	double value = 0.;

	Double_t T1 = dynamic_cast<const RooProdPdf*>(_FuncList.at(0))->getVal(RooArgSet(*mass.absArg(),*dbkg.absArg()));
	Double_t T2 = dynamic_cast<const RooProdPdf*>(_FuncList.at(1))->getVal(RooArgSet(*mass.absArg(),*dbkg.absArg()));
	Double_t T3 = dynamic_cast<const RooProdPdf*>(_FuncList.at(2))->getVal(RooArgSet(*mass.absArg(),*dbkg.absArg()));

	Double_t T1_coef = dynamic_cast<const RooAbsReal*>(_coefList.at(0))->getVal();
	Double_t T2_coef = dynamic_cast<const RooAbsReal*>(_coefList.at(1))->getVal();
	Double_t T3_coef = dynamic_cast<const RooAbsReal*>(_coefList.at(2))->getVal();

	value = T1*T1_coef + T2*T2_coef+T3*T3_coef; 

	if ( value <= 0.) { return 1.0e-200;}

	return value ; 

} 

Int_t HZZ4L_onshell_2D::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{

	if (matchArgs(allVars,analVars,RooArgSet(*mass.absArg(),*dbkg.absArg()))) return 4 ;

	return 0 ;

}

Double_t HZZ4L_onshell_2D::analyticalIntegral(Int_t code, const char* rangeName) const
{
	switch(code)
	{

		case 4: 
			{
				Double_t T1_coef = dynamic_cast<const RooAbsReal*>(_coefList.at(0))->getVal();
				Double_t T2_coef = dynamic_cast<const RooAbsReal*>(_coefList.at(1))->getVal();
				Double_t T3_coef = dynamic_cast<const RooAbsReal*>(_coefList.at(2))->getVal();
				double integral = T1_coef+ T2_coef+ T3_coef; 
				return integral;
			}

	}

	assert(0) ;
	return 0 ;
}

