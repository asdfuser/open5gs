#ifndef __EMM_BUILD_H__
#define __EMM_BUILD_H__

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) emm_build_attach_accept(pkbuf_t **pkbuf, mme_ue_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EMM_BUILD_H__ */
