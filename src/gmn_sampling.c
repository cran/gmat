#include <stdlib.h>
#include <string.h>

#include "gmn_sampling.h"


/*
 * Selective Gram Schmidt algorithm 
 *
 * @param span_ort Matrix with rows containing the orthogonal vectors
 * @param span Matrix with rows containing the vectors to orthogonalize
 * @param nvec Number of vectors to orthogonalize (rows of span)
 * @param dim Dimension of vectors (columns of span)
 */
int gram_schmidt_sel (double *mort, int *madj, double *mcov, 
		unsigned int *dim) {
	double **span_sel = NULL, **ort_base = NULL;
	double *v_proj = NULL;
	unsigned int i = 0, j = 0, k = 0;
	unsigned int n_span = 0, i_current = 0;
	
	if (mort == NULL || madj == NULL || mcov == NULL || dim == NULL) {
		return -1;
	}

	if ((v_proj = calloc(dim[0], sizeof(double))) == NULL) {
		return -1;
	}

	if ((span_sel = calloc(dim[0], sizeof(double *))) == NULL) {
		free(v_proj); v_proj = NULL;
		return -1;
	}

	if ((ort_base = calloc(dim[0], sizeof(double *))) == NULL) {
		free(span_sel); span_sel = NULL;
		free(v_proj); v_proj = NULL;
		return -1;
	}

	for (i = 0; i < dim[0]; i++) {
		ort_base[i] = NULL;
	}

	for (i = 0; i < dim[0]; i++) {
		if ((ort_base[i] = calloc(dim[0], sizeof(double))) == NULL) {
			for (j = 0; j < i; j++) {
				free(ort_base[j]); ort_base[j] = NULL;
			}
			free(v_proj); v_proj = NULL;
			free(span_sel); span_sel = NULL;
			free(ort_base); ort_base = NULL;
			return -1;
		}
	}

	for (i = 0; i < dim[0]; i++) {
	
		i_current = i * dim[0];
		memcpy(mort + i_current, mcov + i_current, sizeof(double) * dim[0]);
		n_span = 0;

		for (j = 0; j < i; j++) {
			if (madj[i_current + j] == 0) {
				span_sel[n_span] = mort + j * dim[0];
				n_span++;
			}
		}

		gram_schmidt(ort_base, span_sel, &n_span, dim);
		for (j = 0; j < n_span; j++) {
			proj_ort(v_proj, mort + i_current, ort_base[j], dim);
			for (k = 0; k < dim[0]; k++) {
				mort[i_current + k] -= v_proj[k];
			}
		}
	}
	
	free(v_proj); v_proj = NULL;
	free(span_sel); span_sel = NULL;
	for (i = 0; i < dim[0]; i++) {
		free(ort_base[i]); ort_base[i] = NULL;
	}
	free(ort_base); ort_base = NULL;

	return 0;
}


/*
 * Gram Schmidt algorithm 
 *
 * @param span_ort Matrix with rows containing the orthogonal vectors
 * @param span Matrix with rows containing the vectors to orthogonalize
 * @param nvec Number of vectors to orthogonalize (rows of span)
 * @param dim Dimension of vectors (columns of span)
 */
int gram_schmidt (double **span_ort, double **span, 
		unsigned int *nvec, unsigned int *dim) 
{
	double *v_proj = NULL;
	unsigned int i = 0, j = 0, k = 0;

	if (span_ort == NULL || span == NULL || nvec == NULL || dim == NULL) {
		return -1;
	}

	for (i = 0; i < nvec[0]; i++) {
		memcpy(span_ort[i], span[i], sizeof(double) * dim[0]);
	}

	if ((v_proj = calloc(dim[0], sizeof(double))) == NULL) {
		return -1;
	}

	for (i = 1; i < nvec[0]; i++) {
		for (j = 0; j < i; j++) {
			proj_ort(v_proj, span_ort[i], span_ort[j], dim);
			for (k = 0; k < dim[0]; k++) {
				span_ort[i][k] -= v_proj[k];
			}
		}
	}

	free(v_proj); v_proj = NULL;

	return 0;
}

/* 
 * Orthogonal projection of two vectors v and u.
 */
int proj_ort (double *v_proj_u, double *v, double *u, unsigned int *dim)
{
	unsigned int i = 0;
	double dot_uv = 0, dot_uu = 0, lambda = 0;

	if (v_proj_u == NULL || v == NULL || u == NULL || dim == NULL) {
		return -1;
	}
	
	for (i = 0; i < dim[0]; i++) {
		dot_uv += (u[i] * v[i]);
		dot_uu += (u[i] * u[i]);
	}

	lambda = dot_uv/dot_uu;

	for (i = 0; i < dim[0]; i++) {
		v_proj_u[i] = lambda * u[i];
	}

	return 0;
}
