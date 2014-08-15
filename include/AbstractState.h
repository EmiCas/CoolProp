/*
 * AbstractState.h
 *
 *  Created on: 21 Dec 2013
 *      Author: jowr
 */

#ifndef ABSTRACTSTATE_H_
#define ABSTRACTSTATE_H_

#include "CachedElement.h"
#include "Exceptions.h"
#include "DataStructures.h"

#include <numeric>

namespace CoolProp {

//! The mother of all state classes
/*!
This class provides the basic properties based on interrelations of the
properties, their derivatives and the Helmholtz energy terms. It does not
provide the mechanism to update the values. This has to be implemented in
a subclass. Most functions are defined as virtual functions allowing us
redefine them later, for example to implement the TTSE technique. The
functions defined here are always used as a fall-back.

This base class does not perform any checks on the two-phase conditions and
alike. Most of the functions defined here only apply to compressible single
state substances. Make sure you are aware of all the assumptions we made
when using this class.

Add build table function to Abstract State
Interpolator inherit AS implemented by TTSE BICUBIC

*/
class AbstractState {
protected:

    /// Some administrative variables
    long _fluid_type;
    phases _phase; ///< The key for the phase from CoolProp::phases enum
    bool _forceSinglePhase, _forceTwoPhase;

    bool isSupercriticalPhase(void){
        return (this->_phase == iphase_supercritical || this->_phase == iphase_supercritical_liquid || this->_phase == iphase_supercritical_gas);
    }
    
    bool isHomogeneousPhase(void){
        return (this->_phase==iphase_liquid || this->_phase==iphase_gas || isSupercriticalPhase());
    }

    bool isTwoPhase(void){
        return (this->_phase==iphase_twophase);
    }

    /// Two important points
    SimpleState _critical, _reducing;

    /// Molar mass [mol/kg]
    CachedElement _molar_mass;

    /// Universal gas constant [J/mol/K]
    CachedElement _gas_constant;

    /// Bulk values
    double _rhomolar, _T, _p, _Q, _R;

    CachedElement _tau, _delta;

    /// Transport properties
    CachedElement _viscosity, _conductivity, _surface_tension;

    CachedElement _hmolar, _smolar, _umolar, _logp, _logrhomolar, _cpmolar, _cvmolar, _speed_sound, _gibbsmolar;

    /// Ancillary values
    CachedElement _rhoLanc, _rhoVanc, _pLanc, _pVanc, _TLanc, _TVanc;

    CachedElement _fugacity_coefficient;

    /// Smoothing values
    double _rhospline, _dsplinedp, _dsplinedh;

    /// Cached low-level elements for in-place calculation of other properties
    CachedElement _alpha0, _dalpha0_dTau, _dalpha0_dDelta, _d2alpha0_dTau2, _d2alpha0_dDelta_dTau,
            _d2alpha0_dDelta2, _d3alpha0_dTau3, _d3alpha0_dDelta_dTau2, _d3alpha0_dDelta2_dTau,
            _d3alpha0_dDelta3, _alphar, _dalphar_dTau, _dalphar_dDelta, _d2alphar_dTau2, _d2alphar_dDelta_dTau,
            _d2alphar_dDelta2, _d3alphar_dTau3, _d3alphar_dDelta_dTau2, _d3alphar_dDelta2_dTau,
            _d3alphar_dDelta3;

    CachedElement _dalphar_dDelta_lim, _d2alphar_dDelta2_lim,
            _d2alphar_dDelta_dTau_lim, _d3alphar_dDelta2_dTau_lim;

    /// Two-Phase variables
    CachedElement _rhoLmolar, _rhoVmolar;

    // ----------------------------------------
    // Property accessors to be optionally implemented by the backend
    // for properties that are not always calculated
    // ----------------------------------------
    /// Using this backend, calculate the molar enthalpy in J/mol
    virtual long double calc_hmolar(void){throw NotImplementedError("calc_hmolar is not implemented for this backend");};
    /// Using this backend, calculate the molar entropy in J/mol/K
    virtual long double calc_smolar(void){throw NotImplementedError("calc_smolar is not implemented for this backend");};
    /// Using this backend, calculate the molar internal energy in J/mol
    virtual long double calc_umolar(void){throw NotImplementedError("calc_umolar is not implemented for this backend");};
    /// Using this backend, calculate the molar constant-pressure specific heat in J/mol/K
    virtual long double calc_cpmolar(void){throw NotImplementedError("calc_cpmolar is not implemented for this backend");};
    /// Using this backend, calculate the molar constant-volume specific heat in J/mol/K
    virtual long double calc_cvmolar(void){throw NotImplementedError("calc_cvmolar is not implemented for this backend");};
    /// Using this backend, calculate the molar Gibbs function in J/mol
    virtual long double calc_gibbsmolar(void){throw NotImplementedError("calc_gibbsmolar is not implemented for this backend");};
    /// Using this backend, calculate the speed of sound in m/s
    virtual long double calc_speed_sound(void){throw NotImplementedError("calc_speed_sound is not implemented for this backend");};
    /// Using this backend, calculate the isothermal compressibility \f$ \kappa = -\frac{1}{v}\left.\frac{\partial v}{\partial p}\right|_T=\frac{1}{\rho}\left.\frac{\partial \rho}{\partial p}\right|_T\f$  in 1/Pa
    virtual long double calc_isothermal_compressibility(void){throw NotImplementedError("calc_isothermal_compressibility is not implemented for this backend");};
    /// Using this backend, calculate the isobaric expansion coefficient \f$ \beta = \frac{1}{v}\left.\frac{\partial v}{\partial T}\right|_p = -\frac{1}{\rho}\left.\frac{\partial \rho}{\partial T}\right|_p\f$  in 1/K
    virtual long double calc_isobaric_expansion_coefficient(void){throw NotImplementedError("calc_isobaric_expansion_coefficient is not implemented for this backend");};
    /// Using this backend, calculate the viscosity in Pa-s
    virtual long double calc_viscosity(void){throw NotImplementedError("calc_viscosity is not implemented for this backend");};
    /// Using this backend, calculate the thermal conductivity in W/m/K
    virtual long double calc_conductivity(void){throw NotImplementedError("calc_conductivity is not implemented for this backend");};
    /// Using this backend, calculate the surface tension in N/m
    virtual long double calc_surface_tension(void){throw NotImplementedError("calc_surface_tension is not implemented for this backend");};
    /// Using this backend, calculate the molar mass in kg/mol
    virtual long double calc_molar_mass(void){throw NotImplementedError("calc_molar_mass is not implemented for this backend");};
    /// Using this backend, calculate the pressure in Pa
    virtual long double calc_pressure(void){throw NotImplementedError("calc_pressure is not implemented for this backend");};
    /// Using this backend, calculate the universal gas constant \f$R_u\f$ in J/mol/K
    virtual long double calc_gas_constant(void){throw NotImplementedError("calc_gas_constant is not implemented for this backend");};
    /// Using this backend, calculate the fugacity coefficient (dimensionless)
    virtual long double calc_fugacity_coefficient(int i){throw NotImplementedError("calc_fugacity_coefficient is not implemented for this backend");};


    // Derivatives of residual helmholtz energy
    /// Using this backend, calculate the residual Helmholtz energy term \f$\alpha^r\f$ (dimensionless)
    virtual long double calc_alphar(void){throw NotImplementedError("calc_alphar is not implemented for this backend");};
    /// Using this backend, calculate the residual Helmholtz energy term \f$\alpha^r_{\delta}\f$ (dimensionless)
    virtual long double calc_dalphar_dDelta(void){throw NotImplementedError("calc_dalphar_dDelta is not implemented for this backend");};
    /// Using this backend, calculate the residual Helmholtz energy term \f$\alpha^r_{\tau}\f$ (dimensionless)
    virtual long double calc_dalphar_dTau(void){throw NotImplementedError("calc_dalphar_dTau is not implemented for this backend");};
    /// Using this backend, calculate the residual Helmholtz energy term \f$\alpha^r_{\delta\delta}\f$ (dimensionless)
    virtual long double calc_d2alphar_dDelta2(void){throw NotImplementedError("calc_d2alphar_dDelta2 is not implemented for this backend");};
    /// Using this backend, calculate the residual Helmholtz energy term \f$\alpha^r_{\delta\tau}\f$ (dimensionless)
    virtual long double calc_d2alphar_dDelta_dTau(void){throw NotImplementedError("calc_d2alphar_dDelta_dTau is not implemented for this backend");};
    /// Using this backend, calculate the residual Helmholtz energy term \f$\alpha^r_{\tau\tau}\f$ (dimensionless)
    virtual long double calc_d2alphar_dTau2(void){throw NotImplementedError("calc_d2alphar_dTau2 is not implemented for this backend");};

    // Derivatives of ideal-gas helmholtz energy
    /// Using this backend, calculate the ideal-gas Helmholtz energy term \f$\alpha^0\f$ (dimensionless)
    virtual long double calc_alpha0(void){throw NotImplementedError("calc_alpha0 is not implemented for this backend");};
    /// Using this backend, calculate the ideal-gas Helmholtz energy term \f$\alpha^0_{\delta}\f$ (dimensionless)
    virtual long double calc_dalpha0_dDelta(void){throw NotImplementedError("calc_dalpha0_dDelta is not implemented for this backend");};
    /// Using this backend, calculate the ideal-gas Helmholtz energy term \f$\alpha^0_{\tau}\f$ (dimensionless)
    virtual long double calc_dalpha0_dTau(void){throw NotImplementedError("calc_dalpha0_dTau is not implemented for this backend");};
    /// Using this backend, calculate the ideal-gas Helmholtz energy term \f$\alpha^0_{\delta\delta}\f$ (dimensionless)
    virtual long double calc_d2alpha0_dDelta_dTau(void){throw NotImplementedError("calc_d2alpha0_dDelta_dTau is not implemented for this backend");};
    /// Using this backend, calculate the ideal-gas Helmholtz energy term \f$\alpha^0_{\delta\tau}\f$ (dimensionless)
    virtual long double calc_d2alpha0_dDelta2(void){throw NotImplementedError("calc_d2alpha0_dDelta2 is not implemented for this backend");};
    /// Using this backend, calculate the ideal-gas Helmholtz energy term \f$\alpha^0_{\tau\tau}\f$ (dimensionless)
    virtual long double calc_d2alpha0_dTau2(void){throw NotImplementedError("calc_d2alpha0_dTau2 is not implemented for this backend");};

    virtual void calc_reducing_state(void){throw NotImplementedError("calc_reducing_state is not implemented for this backend");};

    /// Using this backend, calculate the maximum temperature in K
    virtual long double calc_Tmax(void){throw NotImplementedError("calc_Tmax is not implemented for this backend");};
	/// Using this backend, calculate the minimum temperature in K
    virtual long double calc_Tmin(void){throw NotImplementedError("calc_Tmin is not implemented for this backend");};
    /// Using this backend, calculate the maximum pressure in Pa
    virtual long double calc_pmax(void){throw NotImplementedError("calc_pmax is not implemented for this backend");};

    /// Using this backend, calculate the 20-year global warming potential (GWP)
    virtual long double calc_GWP20(void){throw NotImplementedError("calc_GWP20 is not implemented for this backend");};
    /// Using this backend, calculate the 100-year global warming potential (GWP)
    virtual long double calc_GWP100(void){throw NotImplementedError("calc_GWP100 is not implemented for this backend");};
    /// Using this backend, calculate the 500-year global warming potential (GWP)
    virtual long double calc_GWP500(void){throw NotImplementedError("calc_GWP500 is not implemented for this backend");};
    /// Using this backend, calculate the ozone depletion potential (ODP)
    virtual long double calc_ODP(void){throw NotImplementedError("calc_ODP is not implemented for this backend");};
    /// Using this backend, calculate the flame hazard
    virtual long double calc_flame_hazard(void){throw NotImplementedError("calc_flame_hazard is not implemented for this backend");};
    /// Using this backend, calculate the health hazard
    virtual long double calc_health_hazard(void){throw NotImplementedError("calc_health_hazard is not implemented for this backend");};
    /// Using this backend, calculate the physical hazard
    virtual long double calc_physical_hazard(void){throw NotImplementedError("calc_physical_hazard is not implemented for this backend");};

    /// Calculate the first partial derivative for the desired derivative
    virtual long double calc_first_partial_deriv(parameters Of, parameters Wrt, parameters Constant){throw NotImplementedError("calc_first_partial_deriv is not implemented for this backend");};
    /// Calculate the second partial derivative using the given backend
    virtual long double calc_second_partial_deriv(parameters Of1, parameters Wrt1, parameters Constant1, parameters Of2, parameters Constant2){throw NotImplementedError("calc_second_partial_deriv is not implemented for this backend");};

    /// Using this backend, calculate the reduced density (rho/rhoc)
    virtual long double calc_reduced_density(void){throw NotImplementedError("calc_reduced_density is not implemented for this backend");};
    /// Using this backend, calculate the reciprocal reduced temperature (Tc/T)
    virtual long double calc_reciprocal_reduced_temperature(void){throw NotImplementedError("calc_reciprocal_reduced_temperature is not implemented for this backend");};

    /// Using this backend, calculate the second virial coefficient
    virtual long double calc_Bvirial(void){throw NotImplementedError("calc_Bvirial is not implemented for this backend");};
    /// Using this backend, calculate the third virial coefficient
    virtual long double calc_Cvirial(void){throw NotImplementedError("calc_Cvirial is not implemented for this backend");};
    /// Using this backend, calculate the derivative dB/dT
    virtual long double calc_dBvirial_dT(void){throw NotImplementedError("calc_dBvirial_dT is not implemented for this backend");};
    /// Using this backend, calculate the derivative dC/dT
    virtual long double calc_dCvirial_dT(void){throw NotImplementedError("calc_dCvirial_dT is not implemented for this backend");};

    /// Using this backend, get the name of the fluid
    virtual std::string calc_name(void){throw NotImplementedError("calc_name is not implemented for this backend");};

    /// Using this backend, get the triple point temperature in K
    virtual long double calc_Ttriple(void){throw NotImplementedError("calc_Ttriple is not implemented for this backend");};
    /// Using this backend, get the triple point pressure in Pa
    virtual long double calc_p_triple(void){throw NotImplementedError("calc_p_triple is not implemented for this backend");};

    /// Using this backend, get the critical point temperature in K
    virtual long double calc_T_critical(void){throw NotImplementedError("calc_T_critical is not implemented for this backend");};
    /// Using this backend, get the critical point pressure in Pa
    virtual long double calc_p_critical(void){throw NotImplementedError("calc_p_critical is not implemented for this backend");};
    /// Using this backend, get the critical point molar density in mol/m^3
    virtual long double calc_rhomolar_critical(void){throw NotImplementedError("calc_rhomolar_critical is not implemented for this backend");};

    /// Using this backend, construct the phase envelope, the variable type describes the type of phase envelope to be built.
    virtual void calc_phase_envelope(const std::string &type){throw NotImplementedError("calc_phase_envelope is not implemented for this backend");};
    
    /// 
    virtual long double calc_rhomass(void){return _rhomolar*molar_mass();}
    virtual long double calc_hmass(void){return hmolar()/molar_mass();}
    virtual long double calc_smass(void){return smolar()/molar_mass();}
    virtual long double calc_cpmass(void){return cpmolar()/molar_mass();}
    virtual long double calc_cvmass(void){return cvmolar()/molar_mass();}
    virtual long double calc_umass(void){return umolar()/molar_mass();}
    
    /// Update the states after having changed the reference state for enthalpy and entropy
    virtual void update_states(void){throw NotImplementedError("This backend does not implement update_states function");};
	
	virtual long double calc_melting_line(int param, int given, long double value){throw NotImplementedError("This backend does not implement calc_melting_line function");};
	
	virtual phases calc_phase(void){throw NotImplementedError("This backend does not implement calc_phase function");};
    
    /// Using this backend, calculate a phase given by the state string
    /// @param state A string that describes the state desired, one of "hs_anchor", "critical"/"crit", "reducing"
    virtual const CoolProp::SimpleState & calc_state(const std::string &state){throw NotImplementedError("calc_state is not implemented for this backend");};

public:

    virtual long double calc_melt_p_T(long double T){throw NotImplementedError("calc_melt_p_T is not implemented for this backend");};
    virtual long double calc_melt_T_p(long double p){throw NotImplementedError("calc_melt_T_p is not implemented for this backend");};
    virtual long double calc_melt_rho_T(long double T){throw NotImplementedError("calc_melt_rho_T is not implemented for this backend");};

    AbstractState(){};
    virtual ~AbstractState(){};

    /// A factory function to return a pointer to a new-allocated instance of one of the backends.
    /**
    Very Important!! : Use a smart pointer to manage the pointer returned.  In older versions of C++, you can use std::tr1::smart_ptr. In C++2011 you can use std::shared_ptr

    Several backends are possible:

    1. "?" : The backend is unknown, we will parse the fluid string to determine the backend to be used.  Probably will use HEOS backend (see below)
    2. "HEOS" : The Helmholtz Equation of State backend for use with pure and pseudo-pure fluids, and mixtures, all of which are based on multi-parameter Helmholtz Energy equations of state.  The fluid part of the string should then either be
       1. A pure or pseudo-pure fluid name (eg. "PROPANE" or "R410A"), yielding a HelmholtzEOSBackend instance.
       2. A string that encodes the components of the mixture with a "&" between them (e.g. "R32&R125"), yielding a HelmholtzEOSMixtureBackend instance.

    3. "REFPROP" : The REFPROP backend will be used.  The fluid part of the string should then either be
       1. A pure or pseudo-pure fluid name (eg. "PROPANE" or "R410A"), yielding a REFPROPBackend instance.
       2. A string that encodes the components of the mixture with a "&" between them (e.g. "R32&R125"), yielding a REFPROPMixtureBackend instance.

    4. "TTSE&XXXX": The TTSE backend will be used, and the tables will be generated using the XXXX backend where XXXX is one of the base backends("HEOS", "REFPROP", etc. )
    5. "BICUBIC&XXXX": The Bicubic backend will be used, and the tables will be generated using the XXXX backend where XXXX is one of the base backends("HEOS", "REFPROP", etc. )
    6. "INCOMP": The incompressible backend will be used
    7. "BRINE": The brine backend will be used
    */
    static AbstractState * factory(const std::string &backend, const std::string &fluid_string);

    // The derived classes must implement this function to define whether they use mole fractions (true) or mass fractions (false)
    virtual bool using_mole_fractions(void) = 0;
    virtual bool using_mass_fractions(void) = 0;
    virtual bool using_volu_fractions(void) = 0;

    virtual void update(long input_pair, double Value1, double Value2) = 0;
    virtual void set_mole_fractions(const std::vector<long double> &mole_fractions) = 0;
    virtual void set_mass_fractions(const std::vector<long double> &mass_fractions) = 0;
    virtual void set_volu_fractions(const std::vector<long double> &mass_fractions){throw NotImplementedError("Volume composition has not been implemented.");}
    
    /// Clear all the cached values
    bool clear();

    void set_mole_fractions(const std::vector<double> &mole_fractions){set_mole_fractions(std::vector<long double>(mole_fractions.begin(), mole_fractions.end()));};
    void set_mass_fractions(const std::vector<double> &mass_fractions){set_mass_fractions(std::vector<long double>(mass_fractions.begin(), mass_fractions.end()));};
    void set_volu_fractions(const std::vector<double> &volu_fractions){set_volu_fractions(std::vector<long double>(volu_fractions.begin(), volu_fractions.end()));};

    const CoolProp::SimpleState & get_reducing_state(){return _reducing;};
    const CoolProp::SimpleState & get_state(const std::string &state){return calc_state(state);};

    double keyed_output(int key);
	double trivial_keyed_output(int key);
     
    /** \brief The first partial derivative in homogeneous phases
     * 
     * \f[ \left(\frac{\partial A}{\partial B}\right)_C = \frac{\left(\frac{\partial A}{\partial \tau}\right)_\delta\left(\frac{\partial C}{\partial \delta}\right)_\tau-\left(\frac{\partial A}{\partial \delta}\right)_\tau\left(\frac{\partial C}{\partial \tau}\right)_\delta}{\left(\frac{\partial B}{\partial \tau}\right)_\delta\left(\frac{\partial C}{\partial \delta}\right)_\tau-\left(\frac{\partial B}{\partial \delta}\right)_\tau\left(\frac{\partial C}{\partial \tau}\right)_\delta} = \frac{N}{D}\f]
     */
    long double first_partial_deriv(parameters Of, parameters Wrt, parameters Constant){return calc_first_partial_deriv(Of, Wrt, Constant);};
    
    /** \brief The second partial derivative in homogeneous phases
     * 
     *  \sa \ref CoolProp::AbstractState::first_partial_deriv
     * 
     * \f[
     * \frac{\partial}{\partial D}\left(\left(\frac{\partial A}{\partial B}\right)_C\right)_E = \frac{\frac{\partial}{\partial \tau}\left( \left(\frac{\partial A}{\partial B}\right)_C \right)_\delta\left(\frac{\partial E}{\partial \delta}\right)_\tau-\frac{\partial}{\partial \delta}\left(\left(\frac{\partial A}{\partial B}\right)_C\right)_\tau\left(\frac{\partial E}{\partial \tau}\right)_\delta}{\left(\frac{\partial D}{\partial \tau}\right)_\delta\left(\frac{\partial E}{\partial \delta}\right)_\tau-\left(\frac{\partial D}{\partial \delta}\right)_\tau\left(\frac{\partial E}{\partial \tau}\right)_\delta}
     * \f]
     * 
     * which can be expressed in parts as
     * 
     * \f[\left(\frac{\partial N}{\partial \delta}\right)_{\tau} = \left(\frac{\partial A}{\partial \tau}\right)_\delta\left(\frac{\partial^2 C}{\partial \delta^2}\right)_{\tau}+\left(\frac{\partial^2 A}{\partial \tau\partial\delta}\right)\left(\frac{\partial C}{\partial \delta}\right)_{\tau}-\left(\frac{\partial A}{\partial \delta}\right)_\tau\left(\frac{\partial^2 C}{\partial \tau\partial\delta}\right)-\left(\frac{\partial^2 A}{\partial \delta^2}\right)_{\tau}\left(\frac{\partial C}{\partial \tau}\right)_\delta\f]
     * \f[\left(\frac{\partial D}{\partial \delta}\right)_{\tau} = \left(\frac{\partial B}{\partial \tau}\right)_\delta\left(\frac{\partial^2 C}{\partial \delta^2}\right)_{\tau}+\left(\frac{\partial^2 B}{\partial \tau\partial\delta}\right)\left(\frac{\partial C}{\partial \delta}\right)_{\tau}-\left(\frac{\partial B}{\partial \delta}\right)_\tau\left(\frac{\partial^2 C}{\partial \tau\partial\delta}\right)-\left(\frac{\partial^2 B}{\partial \delta^2}\right)_{\tau}\left(\frac{\partial C}{\partial \tau}\right)_\delta\f]
     * \f[\left(\frac{\partial N}{\partial \tau}\right)_{\delta} = \left(\frac{\partial A}{\partial \tau}\right)_\delta\left(\frac{\partial^2 C}{\partial \delta\partial\tau}\right)+\left(\frac{\partial^2 A}{\partial \tau^2}\right)_\delta\left(\frac{\partial C}{\partial \delta}\right)_{\tau}-\left(\frac{\partial A}{\partial \delta}\right)_\tau\left(\frac{\partial^2 C}{\partial \tau^2}\right)_\delta-\left(\frac{\partial^2 A}{\partial \delta\partial\tau}\right)\left(\frac{\partial C}{\partial \tau}\right)_\delta\f]
     * \f[\left(\frac{\partial D}{\partial \tau}\right)_{\delta} = \left(\frac{\partial B}{\partial \tau}\right)_\delta\left(\frac{\partial^2 C}{\partial \delta\partial\tau}\right)+\left(\frac{\partial^2 B}{\partial \tau^2}\right)_\delta\left(\frac{\partial C}{\partial \delta}\right)_{\tau}-\left(\frac{\partial B}{\partial \delta}\right)_\tau\left(\frac{\partial^2 C}{\partial \tau^2}\right)_\delta-\left(\frac{\partial^2 B}{\partial \delta\partial\tau}\right)\left(\frac{\partial C}{\partial \tau}\right)_\delta\f]
     * \f[\frac{\partial}{\partial \tau}\left( \left(\frac{\partial A}{\partial B}\right)_C \right)_\delta = \frac{D\left(\frac{\partial N}{\partial \tau}\right)_{\delta}-N\left(\frac{\partial D}{\partial \tau}\right)_{\delta}}{D^2}\f]
     * \f[\frac{\partial}{\partial \delta}\left( \left(\frac{\partial A}{\partial B}\right)_C \right)_\tau = \frac{D\left(\frac{\partial N}{\partial \delta}\right)_{\tau}-N\left(\frac{\partial D}{\partial \delta}\right)_{\tau}}{D^2}\f]
     */
    long double second_partial_deriv(parameters Of1, parameters Wrt1, parameters Constant1, parameters Of2, parameters Constant2){return calc_second_partial_deriv(Of1,Wrt1,Constant1,Of2,Constant2);};

    // Limits
	double Tmin(void);
    double Tmax(void);
    double pmax(void);
    double Ttriple(void);
	
	phases phase(void){return calc_phase();};

    /// Return the critical temperature in K
    /**
    For pure fluids, this is the critical point temperature
    For mixtures, it is the exact critical point temperature calculated by the methods of Michelsen( \todo fill in reference)
    */
    double T_critical(void);
    /// Return the critical pressure in Pa
    /**
    For pure fluids, this is the critical point pressure as defined by the author of the EOS
    For mixtures, it is the exact critical point pressure calculated by the methods of Michelsen( \todo fill in reference)
    */
    double p_critical(void);
    /// Return the critical molar density in mol/m^3
    /**
    For pure fluids, this is the critical point molar density
    For mixtures, it is the exact critical point molar density calculated by the methods of Michelsen( \todo fill in reference)
    */
    double rhomolar_critical(void);
    
    /// Return the triple point pressure 
    double p_triple(void);

    std::string name(){return calc_name();};

    // ----------------------------------------
    // Bulk properties - temperature and density are directly calculated every time
    // All other parameters are calculated on an as-needed basis
    // ----------------------------------------
    /// Return the temperature in K
    double T(void)  {return _T;};
    /// Return the molar density in mol/m^3
    double rhomolar(void){return _rhomolar;};
    /// Return the mass density in kg/m^3
    double rhomass(void){return calc_rhomass();};
    /// Return the pressure in Pa
    double p(void)  {return _p;};
    /// Return the vapor quality (mol/mol); Q = 0 for saturated liquid
    double Q(void)  {return _Q;};

    /// Return the reciprocal of the reduced temperature (\f$\tau = T_c/T\f$)
    double tau(void);
    /// Return the reduced density (\f$\delta = \rho/\rho_c\f$)
    double delta(void);
    /// Return the molar mass in kg/mol
    double molar_mass(void);
    /// Return the mole-fraction weighted gas constant in J/mol/K
    double gas_constant(void);

    double Bvirial(void);
    double dBvirial_dT(void);
    double Cvirial(void);
    double dCvirial_dT(void);

    /// Return the molar enthalpy in J/mol
    double hmolar(void);
    /// Return the mass enthalpy in J/kg
    double hmass(void){return calc_hmass();};
    /// Return the molar entropy in J/mol/K
    double smolar(void);
    /// Return the molar entropy in J/kg/K
    double smass(void){return calc_smass();};
    /// Return the molar internal energy in J/mol
    double umolar(void);
    /// Return the mass internal energy in J/kg
    double umass(void){return calc_umass();};
    /// Return the molar constant pressure specific heat in J/mol/K
    double cpmolar(void);
    /// Return the mass constant pressure specific heat in J/kg/K
    double cpmass(void){return calc_cpmass();};
    /// Return the molar constant volume specific heat in J/mol/K
    double cvmolar(void);
    /// Return the mass constant volume specific heat in J/kg/K
    double cvmass(void){return calc_cvmass();};
    /// Return the Gibbs function in J/mol
    double gibbsmolar(void){return calc_gibbsmolar();};
    /// Return the speed of sound in m/s
    double speed_sound(void);
    /// Return the isothermal compressibility \f$ \kappa = -\frac{1}{v}\left.\frac{\partial v}{\partial p}\right|_T=\frac{1}{\rho}\left.\frac{\partial \rho}{\partial p}\right|_T\f$  in 1/Pa
    double isothermal_compressibility(void);
    /// Return the isobaric expansion coefficient \f$ \beta = \frac{1}{v}\left.\frac{\partial v}{\partial T}\right|_p = -\frac{1}{\rho}\left.\frac{\partial \rho}{\partial T}\right|_p\f$  in 1/K
    double isobaric_expansion_coefficient(void);
    double fugacity_coefficient(int i);
    void build_phase_envelope(const std::string &type);
    //double fundamental_derivative_of_gas_dynamics(void);
	
    /// Return true if the fluid has a melting line - default is false, but can be re-implemented by derived class
    virtual bool has_melting_line(void){return false;};
    
    /// Return a value from the melting line
    /// @param param The key for the parameter to be returned
    /// @param given The key for the parameter that is given
    /// @param value The value for the parameter that is given
	double melting_line(int param, int given, double value);
    
    /// Return the value from a saturation ancillary curve (if the backend implements it)
    /// @param param The key for the parameter to be returned
    /// @param Q The quality for the parameter that is given (0 = saturated liquid, 1 = saturated vapor)
    /// @param given The key for the parameter that is given
    /// @param value The value for the parameter that is given
    //double saturation_ancillary(int param, int Q, int given, double value);

    // ----------------------------------------
    // Transport properties
    // ----------------------------------------
    /// Return the viscosity in Pa-s
    double viscosity(void);
    /// Return the thermal conductivity in W/m/K
    double conductivity(void);
    /// Return the surface tension in N/m
    double surface_tension(void);

    // ----------------------------------------
    // Helmholtz energy and derivatives
    // ----------------------------------------
    /// Return the term \f$ \alpha^0 \f$
    long double alpha0(void){
        if (!_alpha0) _alpha0 = calc_alpha0();
        return _alpha0;
    };
    long double dalpha0_dDelta(void){
        if (!_dalpha0_dDelta) _dalpha0_dDelta = calc_dalpha0_dDelta();
        return _dalpha0_dDelta;
    };
    long double dalpha0_dTau(void){
        if (!_dalpha0_dTau) _dalpha0_dTau = calc_dalpha0_dTau();
        return _dalpha0_dTau;
    };
    long double d2alpha0_dDelta2(void){
        if (!_d2alpha0_dDelta2) _d2alpha0_dDelta2 = calc_d2alpha0_dDelta2();
        return _d2alpha0_dDelta2;
    };
    long double d2alpha0_dDelta_dTau(void){
        if (!_d2alpha0_dDelta_dTau) _d2alpha0_dDelta_dTau = calc_d2alpha0_dDelta_dTau();
        return _d2alpha0_dDelta_dTau;
    };
    long double d2alpha0_dTau2(void){
        if (!_d2alpha0_dTau2) _d2alpha0_dTau2 = calc_d2alpha0_dTau2();
        return _d2alpha0_dTau2;
    };
    /*
    virtual double d3alpha0_dDelta3(void) = 0;
    virtual double d3alpha0_dDelta2_dTau(void) = 0;
    virtual double d3alpha0_dDelta_dTau2(void) = 0;
    virtual double d3alpha0_dTau3(void) = 0;
    */

    long double alphar(void){
        if (!_alphar) _alphar = calc_alphar();
        return _alphar;
    };
    long double dalphar_dDelta(void){
        if (!_dalphar_dDelta) _dalphar_dDelta = calc_dalphar_dDelta();
        return _dalphar_dDelta;
    };
    long double dalphar_dTau(void){
        if (!_dalphar_dTau) _dalphar_dTau = calc_dalphar_dTau();
        return _dalphar_dTau;
    };
    long double d2alphar_dDelta2(void){
        if (!_d2alphar_dDelta2) _d2alphar_dDelta2 = calc_d2alphar_dDelta2();
        return _d2alphar_dDelta2;
    };
    long double d2alphar_dDelta_dTau(void){
        if (!_d2alphar_dDelta_dTau) _d2alphar_dDelta_dTau = calc_d2alphar_dDelta_dTau();
        return _d2alphar_dDelta_dTau;
    };
    long double d2alphar_dTau2(void){
        if (!_d2alphar_dTau2) _d2alphar_dTau2 = calc_d2alphar_dTau2();
        return _d2alphar_dTau2;
    };
    /*
    virtual double d3alphar_dDelta3(void) = 0;
    virtual double d3alphar_dDelta2_dTau(void) = 0;
    virtual double d3alphar_dDelta_dTau2(void) = 0;
    virtual double d3alphar_dTau3(void) = 0;

    virtual double dalphar_dDelta_lim(void) = 0;
    virtual double d2alphar_dDelta2_lim(void) = 0;
    virtual double d2alphar_dDelta_dTau_lim(void) = 0;
    virtual double d3alphar_dDelta2_dTau_lim(void) = 0;
    */
};

} /* namespace CoolProp */
#endif /* ABSTRACTSTATE_H_ */
