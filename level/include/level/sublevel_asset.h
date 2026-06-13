/**
 * @file sublevel_asset.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-22
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef SUBLEVEL_ASSET_H
#define SUBLEVEL_ASSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <level/internal/module.h>
#include <elements/light.h>
#include <library/asset/types.h>
#include <library/containers/cvector.h>
#include <library/string/cstring.h>
#include <math/vector3f.h>
#include <mesh/bulk_mesh_asset.h>
#include <spatial/bvh/bvh.h>


////////////////////////////////////////////////////////////////////////////////
//| sublevel_asset_t, '*' = sublevel_asset
//|=============================================================================
//| OPERATION                   | SUPPORTED
//|=============================================================================
//|    *_def                    | YES
//|    *_is_def                 | YES
//|    *_replicate              |
//|    *_fullswap               |
//|    *_serialize              | YES
//|    *_deserialize            | YES
//|    *_hash                   |
//|    *_is_equal               |
//|    *_type_size              | YES
//|    *_type_alignment         |
//|    *_type_id_count          |
//|    *_type_ids               |
//|    *_owns_alloc             | YES
//|    *_get_alloc              | YES
//|    *_cleanup                | YES
//|    *_get_dir                | YES
//|    *_get_loader             | YES
//|    *_get_deloader           | YES
//|    *_type_asset_count       | YES
//|    *_type_get_assets        | YES
//|    *_is_asset_type          | YES
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct sublevel_metadata_t {
  point3f player_start;
  float player_angle;
} sublevel_metadata_t;

typedef
struct sublevel_asset_t {
  cstring_t name;
  sublevel_metadata_t metadata;
  point3f position;
  bulk_mesh_asset_t meshes;
  bvh_t bvh;
  cvector_t lights;
} sublevel_asset_t;

LEVEL_API
void
sublevel_asset_def(void *ptr);

LEVEL_API
uint32_t
sublevel_asset_is_def(const void *ptr);

LEVEL_API
void
sublevel_asset_serialize(
  const void *src,
  binary_stream_t *stream);

LEVEL_API
void
sublevel_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t* stream);

LEVEL_API
size_t
sublevel_asset_type_size(void);

LEVEL_API
uint32_t
sublevel_asset_owns_alloc(void);

LEVEL_API
const allocator_t *
sublevel_asset_get_alloc(const void *ptr);

LEVEL_API
void
sublevel_asset_cleanup(
  void *ptr,
  const allocator_t *allocator);

LEVEL_API
const char *
sublevel_asset_get_dir(void);

LEVEL_API
loader_t
sublevel_asset_get_loader(void);

LEVEL_API
deloader_t
sublevel_asset_get_deloader(void);

LEVEL_API
uint32_t
sublevel_asset_type_asset_count(const void *src);

LEVEL_API
void
sublevel_asset_type_get_assets(const void *src, const asset_ref_t *refs[]);

LEVEL_API
uint32_t
sublevel_asset_is_asset_type(void);

#ifdef __cplusplus
}
#endif

#endif