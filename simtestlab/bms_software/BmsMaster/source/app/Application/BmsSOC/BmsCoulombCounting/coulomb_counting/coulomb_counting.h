#ifndef COULOMB_COUNTING_H
#define COULOMB_COUNTING_H

/**
 * Estimate SOC (%) from coulomb counting.
 * 
 * @param capacity_mAh Total cell capacity in mAh.
 * @param used_mAh Discharged mAh.
 * @return SOC in percentage (0.0 to 100.0)
 */
float EstimateSocFromCoulombs(float capacity_mAh, float used_mAh);

#endif // COULOMB_COUNTING_H
