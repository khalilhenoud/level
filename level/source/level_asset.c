/**
 * @file level_asset.c
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-22
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <assert.h>
#include <level/level_asset.h>
#include <level/sublevel_asset.h>
#include <library/allocator/allocator.h>
#include <library/asset/asset_ref.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
level_asset_def(void *ptr)
{
  assert(ptr);

  {
    level_asset_t *level = (level_asset_t *)ptr;
    memset(level, 0, sizeof(level_asset_t));
  }
}

uint32_t
level_asset_is_def(const void *ptr)
{
  assert(ptr);

  {
    const level_asset_t *level = (const level_asset_t *)ptr;
    level_asset_t def;
    level_asset_def(&def);
    return !memcmp(level, &def, sizeof(level_asset_t));
  }
}

void
level_asset_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const level_asset_t *level = (const level_asset_t *)src;
    cvector_serialize(&level->sublevels, stream);
  }
}

void
level_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    level_asset_t *level = (level_asset_t *)dst;
    cvector_deserialize(&level->sublevels, allocator, stream);
  }
}

size_t
level_asset_type_size(void)
{
  return sizeof(level_asset_t);
}

uint32_t
level_asset_owns_alloc(void)
{
  return 0;
}

const allocator_t *
level_asset_get_alloc(const void *ptr)
{
  return NULL;
}

void
level_asset_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !level_asset_is_def(ptr));
  assert(allocator);

  {
    level_asset_t *level = (level_asset_t *)ptr;
    cvector_cleanup2(&level->sublevels);
  }
}

const char *
level_asset_get_dir(void)
{
  static const char *directory = "levels";
  return directory;
}

static
void
level_asset_loader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(level_asset_t));

  {
    level_asset_t **ptr = (level_asset_t **)ptr_addr;
    level_asset_t *asset_ptr = *ptr;
    binary_stream_t *stream = binary_stream_from_file(
      asset_ref->path.str, allocator);
    *ptr = allocator->mem_alloc(sizeof(level_asset_t));
    level_asset_def(asset_ptr);
    level_asset_deserialize(asset_ptr, allocator, stream);
    binary_stream_cleanup(stream);
    allocator->mem_free(stream);
  }
}

static
void
level_asset_deloader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(level_asset_t));

  {
    level_asset_t **ptr = (level_asset_t **)ptr_addr;
    level_asset_t *asset_ptr = *ptr;
    level_asset_cleanup(asset_ptr, allocator);
    allocator->mem_free(asset_ptr);
    *ptr = NULL;
  }
}

loader_t
level_asset_get_loader(void)
{
  return level_asset_loader;
}

deloader_t
level_asset_get_deloader(void)
{
  return level_asset_deloader;
}

uint32_t
level_asset_type_asset_count(const void *src)
{
  assert(src);

  {
    const level_asset_t *level = src;
    const sublevel_asset_t *sublevel = NULL;
    uint32_t total = 0;
    for (uint32_t i = 0; i < level->sublevels.size; ++i) {
      sublevel = cvector_as_c(&level->sublevels, i, sublevel_asset_t);
      total += sublevel_asset_type_asset_count(sublevel);
    }
    return total;
  }

  return 0;
}

void
level_asset_type_get_assets(const void *src, const asset_ref_t *refs[])
{
  assert(src);

  {
    const level_asset_t *level = src;
    const sublevel_asset_t *sublevel = NULL;
    uint32_t total = 0;
    for (uint32_t i = 0; i < level->sublevels.size; ++i) {
      sublevel = cvector_as_c(&level->sublevels, i, sublevel_asset_t);
      sublevel_asset_type_get_assets(sublevel, refs + total);
      total += sublevel_asset_type_asset_count(sublevel);
    }
  }
}

uint32_t
level_asset_is_asset_type(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_level_asset_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = level_asset_def;
  vtable.fn_is_def = level_asset_is_def;
  vtable.fn_serialize = level_asset_serialize;
  vtable.fn_deserialize = level_asset_deserialize;
  vtable.fn_type_size = level_asset_type_size;
  vtable.fn_owns_alloc = level_asset_owns_alloc;
  vtable.fn_get_alloc = level_asset_get_alloc;
  vtable.fn_cleanup = level_asset_cleanup;
  vtable.fn_get_dir = level_asset_get_dir;
  vtable.fn_get_loader = level_asset_get_loader;
  vtable.fn_get_deloader = level_asset_get_deloader;
  vtable.fn_type_asset_count = level_asset_type_asset_count;
  vtable.fn_type_get_assets = level_asset_type_get_assets;
  vtable.fn_is_asset_type = level_asset_is_asset_type;
  register_type(get_type_id(level_asset_t), &vtable);
}