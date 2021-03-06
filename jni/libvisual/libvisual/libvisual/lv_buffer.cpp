/* Libvisual - The audio visualisation framework.
 *
 * Copyright (C) 2004, 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "config.h"
#include "lv_buffer.h"
#include "lv_common.h"

namespace LV {

  class Buffer::Impl
  {
  public:

      void*       data;
      std::size_t size;
      bool        is_owner;

      Impl ()
          : data (0)
          , size (0)
          , is_owner (false)
      {}

      ~Impl ()
      {
          if (is_owner) {
              visual_mem_free (data);
          }
      }
  };

  Buffer::Buffer ()
      : m_impl (new Impl)
      , m_ref_count (1)
  {
  }

  Buffer::Buffer (void* data, std::size_t size, bool own)
      : m_impl (new Impl)
      , m_ref_count (1)
  {
      m_impl->data = data;
      m_impl->size = size;
      m_impl->is_owner = own;
  }

  Buffer::Buffer (std::size_t size)
      : m_impl (new Impl)
      , m_ref_count (1)
  {
      m_impl->size = size;
      allocate_data ();
  }

  Buffer::~Buffer ()
  {
      // empty
  }

  void Buffer::destroy_content ()
  {
      if (m_impl->data && m_impl->is_owner)
          visual_mem_free (m_impl->data);

      m_impl->is_owner = false;
      m_impl->data = 0;
      m_impl->size = 0;
  }

  void Buffer::set (void* data, std::size_t size)
  {
      m_impl->data = data;
      m_impl->size = size;
      m_impl->is_owner = false;
  }

  void Buffer::set_size (std::size_t size)
  {
      m_impl->size = size;
  }

  void Buffer::set_data (void *data)
  {
      // FIXME: Deallocate data if unnecessary

      m_impl->data = data;
      m_impl->is_owner = false;
  }

  void Buffer::allocate_data ()
  {
      if (m_impl->data && m_impl->is_owner)
          visual_mem_free (m_impl->data);

      m_impl->data = visual_mem_malloc0 (m_impl->size);
      m_impl->is_owner = true;
  }

  void* Buffer::get_data () const
  {
      return m_impl->data;
  }

  void* Buffer::get_data (std::size_t offset) const
  {
      return static_cast<uint8_t*> (m_impl->data) + offset;
  }

  std::size_t Buffer::get_size () const
  {
      return m_impl->size;
  }

  bool Buffer::is_allocated () const
  {
      return m_impl->is_owner;
  }

  void Buffer::copy (VisBuffer const& src)
  {
      set_size (src.m_impl->size);
      allocate_data ();

      visual_mem_copy (m_impl->data, src.m_impl->data, src.m_impl->size);
  }

  void Buffer::copy_data_to (void* dest)
  {
      visual_return_if_fail (dest != NULL);

      visual_mem_copy (dest, m_impl->data, m_impl->size);
  }

  void Buffer::put (VisBuffer const& src, std::size_t offset)
  {
      put (src.m_impl->data, src.m_impl->size, offset);
  }

  void Buffer::put (void const* data, std::size_t size, std::size_t offset)
  {
      visual_return_if_fail (offset < m_impl->size);

      std::size_t amount = m_impl->size;
      if (offset + size > m_impl->size)
          amount = m_impl->size - offset;

      visual_mem_copy (static_cast<uint8_t*> (m_impl->data) + offset, data, amount);
  }

  void Buffer::fill (uint8_t value)
  {
      visual_mem_set (m_impl->data, value, m_impl->size);
  }

  void Buffer::fill_with_pattern (void const* data, std::size_t size)
  {
      visual_return_if_fail (data != NULL);

      for (std::size_t offset = 0; offset < m_impl->size; offset += size)
          put (data, size, offset);
  }

  void Buffer::ref ()
  {
      m_ref_count++;
  }

  void Buffer::unref ()
  {
      m_ref_count--;
      if (m_ref_count == 0) {
          delete this;
      }
  }

} // LV namespace
