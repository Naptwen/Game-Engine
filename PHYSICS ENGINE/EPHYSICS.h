#pragma once
#include <cmath>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define ke 8.987551781 * pow(10,9)
#define gravity 9.81
#define gravity_constant 6.674 * pow(10,-11)
#define speed_of_light 299792458
#define cosmo_constant 1.1056 * pow(10, -52)
#define photon_mass 0
#define Atom_1 1
#define Atom_torr 760
#define Atom_Pa 101325
#define water_p  997
#define Constant_R  0.0821
#define Constant_Kb  1.38 * pow(10,-23)
#define one_mol 6.02214076 * pow(10, 23)
#define PI 3.141592653589793238
#define magnetic_constant 4 * PI * pow(10,-7)
#define one_column 1.602 * pow(10, -12)
#define electric_charge 8.8541878128 *pow(10,-12)
#define Boltzman_constant 1.38 * pow(10, -23)
#define mass_electron 9.10938365 * pow(10, -31)
#define Planck_constant 6.62607015 * pow(10,-34)
//whole units are atm, m, s, kelvin, kg, Jule
//__________THE SPECIFIC
namespace EPHYSICS {
	struct PHYSICS
	{
		float mass = 0;
		float vx = 0;
		float vy = 0;
		float vz = 0;
	};


	//Dynamic
	float F(float m, float a) { return m * a; }
	float F_spring(float k, float x) { return k * x; }
	float F_gravity(float m1, float m2, float r, float G = gravity_constant) { return G * m1 * m2 * 2 / (r * r); }
	float P(float m, float v) { return m * v; }
	float Impulse(float m, float dv, float dt) { return m * dv / dt; }
	float displacement(float v, float t) { return v * t; }
	float velocity(float a, float t) { return a * t; }
	float friction(float mu, float N) { return mu * N; }

	float Potential_E(float m, float h, float g = gravity) { return m * g * h; }
	float Kinetic_E(float v, float m) { return v * v / 2 * m; }
	float Spring_E(float k, float x) { return x * x / 2 * k; }
	float Work(float dE, float t) { return dE / t; }

	float T(float f) { return 1 / f; }
	float Speed_of_T(float T, float mu) { return sqrt(T / mu); }
	float angular_v(float v, float r) { return v / r; }
	float angular_Force(float r, float F) { return r * F; }
	float angular_p(float r, float p) { return r * p; }
	float angular_work(float F, float ds) { return F * ds; }

	float fluid_density(float m, float V) { return m / V; }
	float fluid_pressure(float F, float A) { return F / A; }
	float fluid_buyancy(float V, float p = water_p, float g = gravity) { return p * g * V; }
	float fluid_Benoulli(float p, float v, float h, float g = gravity) { return p + p * v * v / 2 + p * g * h; }

	float wave_v(float K, float p) { return sqrt(K / p); }
	float wave_v_byT(float T) { return 331.3 * sqrt(1 + T / 273.15); }

	//Thermal dynamic
	float Convert_C2F(float C) { return 9 / 5 * C + 32; }
	float Convert_C2K(float C) { return C + 273.15; }
	float Thermal_L(float L, float alpha, float dT) { return alpha * L * dT; }
	float Thermal_V(float V, float beta, float dT) { return beta * V * dT; }
	float Heat_eff(float Qh, float QL) { return (Qh - QL) / Qh; }
	float Heat_out_cop(float Qh, float QL) { return (Qh) / (Qh - QL); }
	float dEntropy(float dQ, float T) { return dQ / T; }
	//Airo dynamic
	float PVisNRT(float n, float R, float T) { return n * R * T; }
	float PVisNKbT(float N, float Kb, float T) { return N * Kb * T; }
	float NRTisPV(float P, float V) { return P * V; }


	//Electronic and magnetic
	float V(float* V, float* I, float* R)
	{
		if (*V == NULL) return (*V) = (*I) * (*R);
		if (*I == NULL) return *I = *V / (*R);
		if (*R == NULL) return *R = *V / (*I);
		return NULL;
	}
	float Electric_Point_charge_Force(float q1, float q2, float r) { return Boltzman_constant * q1 * q2; }
	float Electric_Filed(float q, float r) { return Boltzman_constant * q / (r * r); }
	float Electric_E(float q, float dV) { return q * dV; }
	float Electric_U(float q, float r) { return Boltzman_constant * q / r; }
	float Electric_P(float I, float V) { return I * V; }
	float Electric_Charge_capacitor(float t, float R, float C) { return electric_charge * (1 - exp(-t / (R * C))); }
	float Electric_disCharge_capacitor(float V0, float t, float R, float C) { return V0 * exp(-t / (R * C)); }
	float Electric_Current_capacitor(float I, float t, float R, float C) { return I * exp(-t / (R * C)); }
	float R_by_p_l_A(float p, float l, float A) { return p * l / A; }


	float current_density(float I, float S) { return I / S; }
	float electric_flux(float E, float dA) { return E * dA; }
	float magnatrix_flux(float B, float dA) { return B * dA; }
	float Guassian_law(float Q_enc) { return Q_enc / electric_charge; }
	float Coulomb_law(float q1, float q2, float r, float k = ke) { return k * q1, q2 / (r * r); }
	float Faraday_law(float dfluxB, float dt) { return -dfluxB / dt; }
	float Ampere_law(float I_enc, float delectric_flux, float dt) { return magnetic_constant * I_enc + magnetic_constant / dt * electric_charge * delectric_flux; }
	//Special_relativity;
	float Energy_wave(float m, float p, float c = speed_of_light) { return m * m * c * c * c * c + p * p * c * c; }
	float Lorentz(float v, float c = speed_of_light) { return 1 / sqrt(1 - v * v / (c * c)); }
	float SR_L(float L, float r) { return L / r; }
	float SR_T(float t, float r) { return t * r; }
	//General relativity
	long double Einsutain_eq(long double R, long double Rv, long double gv, long double lamda = cosmo_constant) { return Rv - R * gv / 2 + lamda * gv; }
	//Quantum mechanics
	float matter_wave(float p, float h = Planck_constant) { return h / p; }
	float Potential_well(float L, float n, float x) { return sqrt(2 / L) * sin(n * PI / L * x); }
}