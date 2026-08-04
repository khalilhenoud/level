/**
 * @file sublevel_asset.c
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-22
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <assert.h>
#include <string.h>
#include <level/sublevel_asset.h>
#include <library/allocator/allocator.h>
#include <library/asset/asset_ref.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
sublevel_asset_def(void *ptr)
{
  assert(ptr);

  {
    sublevel_asset_t *sublevel = (sublevel_asset_t *)ptr;
    memset(sublevel, 0, sizeof(sublevel_asset_t));
  }
}

uint32_t
sublevel_asset_is_def(const void *ptr)
{
  assert(ptr);

  {
    const sublevel_asset_t *sublevel = (const sublevel_asset_t *)ptr;
    sublevel_asset_t def;
    sublevel_asset_def(&def);
    return !memcmp(sublevel, &def, sizeof(sublevel_asset_t));
  }
}

void
sublevel_asset_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const sublevel_asset_t *sublevel = (const sublevel_asset_t *)src;
    cstring_serialize(&sublevel->name, stream);
    binary_stream_write(
      stream, &sublevel->metadata, sizeof(sublevel_metadata_t));
    binary_stream_write(stream, &sublevel->transform, sizeof(matrix4f));
    bulk_mesh_asset_serialize(&sublevel->meshes, stream);
    bvh_serialize(&sublevel->bvh, stream);
    cvector_serialize(&sublevel->lights, stream);
  }
}

void
sublevel_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    sublevel_asset_t *sublevel = (sublevel_asset_t *)dst;
    cstring_def(&sublevel->name);
    cstring_deserialize(&sublevel->name, allocator, stream);
    binary_stream_read2(
      stream, (uint8_t *)&sublevel->metadata, sizeof(sublevel_asset_t));
    binary_stream_read2(
      stream, (uint8_t *)&sublevel->transform, sizeof(matrix4f));
    bulk_mesh_asset_deserialize(&sublevel->meshes, allocator, stream);
    bvh_deserialize(&sublevel->bvh, allocator, stream);
    cvector_deserialize(&sublevel->lights, allocator, stream);
  }
}

size_t
sublevel_asset_type_size(void)
{
  return sizeof(sublevel_asset_t);
}

uint32_t
sublevel_asset_owns_alloc(void)
{
  return 0;
}

const allocator_t *
sublevel_asset_get_alloc(const void *ptr)
{
  return NULL;
}

void
sublevel_asset_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !sublevel_asset_is_def(ptr));
  assert(allocator);

  {
    sublevel_asset_t *sublevel = (sublevel_asset_t *)ptr;
    cstring_cleanup2(&sublevel->name);
    bulk_mesh_asset_cleanup(&sublevel->meshes, allocator);
    bvh_cleanup(&sublevel->bvh, allocator);
    cvector_cleanup2(&sublevel->lights);
  }
}

const char *
sublevel_asset_get_dir(void)
{
  static const char *directory = "sublevels";
  return directory;
}

static
void
sublevel_asset_loader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(sublevel_asset_t));

  {
    sublevel_asset_t **ptr = (sublevel_asset_t **)ptr_addr;
    sublevel_asset_t *asset_ptr = *ptr;
    binary_stream_t *stream = binary_stream_from_file(
      asset_ref->path.str, allocator);
    *ptr = allocator->mem_alloc(sizeof(sublevel_asset_t));
    sublevel_asset_def(asset_ptr);
    sublevel_asset_deserialize(asset_ptr, allocator, stream);
    binary_stream_cleanup(stream);
    allocator->mem_free(stream);
  }
}

static
void
sublevel_asset_deloader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(sublevel_asset_t));

  {
    sublevel_asset_t **ptr = (sublevel_asset_t **)ptr_addr;
    sublevel_asset_t *asset_ptr = *ptr;
    sublevel_asset_cleanup(asset_ptr, allocator);
    allocator->mem_free(asset_ptr);
    *ptr = NULL;
  }
}

loader_t
sublevel_asset_get_loader(void)
{
  return sublevel_asset_loader;
}

deloader_t
sublevel_asset_get_deloader(void)
{
  return sublevel_asset_deloader;
}

uint32_t
sublevel_asset_type_asset_count(const void *src)
{
  assert(src);

  {
    const sublevel_asset_t *sublevel = src;
    return bulk_mesh_asset_type_asset_count(&sublevel->meshes);
  }

  return 0;
}

void
sublevel_asset_type_get_assets(const void *src, const asset_ref_t *refs[])
{
  assert(src);

  {
    const sublevel_asset_t *sublevel = src;
    bulk_mesh_asset_type_get_assets(&sublevel->meshes, refs);
  }
}

uint32_t
sublevel_asset_is_asset_type(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_sublevel_asset_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = sublevel_asset_def;
  vtable.fn_is_def = sublevel_asset_is_def;
  vtable.fn_serialize = sublevel_asset_serialize;
  vtable.fn_deserialize = sublevel_asset_deserialize;
  vtable.fn_type_size = sublevel_asset_type_size;
  vtable.fn_owns_alloc = sublevel_asset_owns_alloc;
  vtable.fn_get_alloc = sublevel_asset_get_alloc;
  vtable.fn_cleanup = sublevel_asset_cleanup;
  vtable.fn_get_dir = sublevel_asset_get_dir;
  vtable.fn_get_loader = sublevel_asset_get_loader;
  vtable.fn_get_deloader = sublevel_asset_get_deloader;
  vtable.fn_type_asset_count = sublevel_asset_type_asset_count;
  vtable.fn_type_get_assets = sublevel_asset_type_get_assets;
  vtable.fn_is_asset_type = sublevel_asset_is_asset_type;
  register_type(get_type_id(sublevel_asset_t), &vtable);
}