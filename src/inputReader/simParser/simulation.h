// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef MOL_SIM_SRC_INPUT_READER_SIM_PARSER_SIMULATION_H
#define MOL_SIM_SRC_INPUT_READER_SIM_PARSER_SIMULATION_H

#ifndef XSD_CXX11
#define XSD_CXX11
#endif

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 4000000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>

#include <xsd/cxx/xml/dom/serialization-header.hxx>
#include <xsd/cxx/tree/serialization.hxx>
#include <xsd/cxx/tree/serialization/byte.hxx>
#include <xsd/cxx/tree/serialization/unsigned-byte.hxx>
#include <xsd/cxx/tree/serialization/short.hxx>
#include <xsd/cxx/tree/serialization/unsigned-short.hxx>
#include <xsd/cxx/tree/serialization/int.hxx>
#include <xsd/cxx/tree/serialization/unsigned-int.hxx>
#include <xsd/cxx/tree/serialization/long.hxx>
#include <xsd/cxx/tree/serialization/unsigned-long.hxx>
#include <xsd/cxx/tree/serialization/boolean.hxx>
#include <xsd/cxx/tree/serialization/float.hxx>
#include <xsd/cxx/tree/serialization/double.hxx>
#include <xsd/cxx/tree/serialization/decimal.hxx>

namespace xml_schema
{
  // anyType and anySimpleType.
  //
  typedef ::xsd::cxx::tree::type type;
  typedef ::xsd::cxx::tree::simple_type< char, type > simple_type;
  typedef ::xsd::cxx::tree::type container;

  // 8-bit
  //
  typedef signed char byte;
  typedef unsigned char unsigned_byte;

  // 16-bit
  //
  typedef short short_;
  typedef unsigned short unsigned_short;

  // 32-bit
  //
  typedef int int_;
  typedef unsigned int unsigned_int;

  // 64-bit
  //
  typedef long long long_;
  typedef unsigned long long unsigned_long;

  // Supposed to be arbitrary-length integral types.
  //
  typedef long long integer;
  typedef long long non_positive_integer;
  typedef unsigned long long non_negative_integer;
  typedef unsigned long long positive_integer;
  typedef long long negative_integer;

  // Boolean.
  //
  typedef bool boolean;

  // Floating-point types.
  //
  typedef float float_;
  typedef double double_;
  typedef double decimal;

  // String types.
  //
  typedef ::xsd::cxx::tree::string< char, simple_type > string;
  typedef ::xsd::cxx::tree::normalized_string< char, string > normalized_string;
  typedef ::xsd::cxx::tree::token< char, normalized_string > token;
  typedef ::xsd::cxx::tree::name< char, token > name;
  typedef ::xsd::cxx::tree::nmtoken< char, token > nmtoken;
  typedef ::xsd::cxx::tree::nmtokens< char, simple_type, nmtoken > nmtokens;
  typedef ::xsd::cxx::tree::ncname< char, name > ncname;
  typedef ::xsd::cxx::tree::language< char, token > language;

  // ID/IDREF.
  //
  typedef ::xsd::cxx::tree::id< char, ncname > id;
  typedef ::xsd::cxx::tree::idref< char, ncname, type > idref;
  typedef ::xsd::cxx::tree::idrefs< char, simple_type, idref > idrefs;

  // URI.
  //
  typedef ::xsd::cxx::tree::uri< char, simple_type > uri;

  // Qualified name.
  //
  typedef ::xsd::cxx::tree::qname< char, simple_type, uri, ncname > qname;

  // Binary.
  //
  typedef ::xsd::cxx::tree::buffer< char > buffer;
  typedef ::xsd::cxx::tree::base64_binary< char, simple_type > base64_binary;
  typedef ::xsd::cxx::tree::hex_binary< char, simple_type > hex_binary;

  // Date/time.
  //
  typedef ::xsd::cxx::tree::time_zone time_zone;
  typedef ::xsd::cxx::tree::date< char, simple_type > date;
  typedef ::xsd::cxx::tree::date_time< char, simple_type > date_time;
  typedef ::xsd::cxx::tree::duration< char, simple_type > duration;
  typedef ::xsd::cxx::tree::gday< char, simple_type > gday;
  typedef ::xsd::cxx::tree::gmonth< char, simple_type > gmonth;
  typedef ::xsd::cxx::tree::gmonth_day< char, simple_type > gmonth_day;
  typedef ::xsd::cxx::tree::gyear< char, simple_type > gyear;
  typedef ::xsd::cxx::tree::gyear_month< char, simple_type > gyear_month;
  typedef ::xsd::cxx::tree::time< char, simple_type > time;

  // Entity.
  //
  typedef ::xsd::cxx::tree::entity< char, ncname > entity;
  typedef ::xsd::cxx::tree::entities< char, simple_type, entity > entities;

  typedef ::xsd::cxx::tree::content_order content_order;
  // Namespace information and list stream. Used in
  // serialization functions.
  //
  typedef ::xsd::cxx::xml::dom::namespace_info< char > namespace_info;
  typedef ::xsd::cxx::xml::dom::namespace_infomap< char > namespace_infomap;
  typedef ::xsd::cxx::tree::list_stream< char > list_stream;
  typedef ::xsd::cxx::tree::as_double< double_ > as_double;
  typedef ::xsd::cxx::tree::as_decimal< decimal > as_decimal;
  typedef ::xsd::cxx::tree::facet facet;

  // Flags and properties.
  //
  typedef ::xsd::cxx::tree::flags flags;
  typedef ::xsd::cxx::tree::properties< char > properties;

  // Parsing/serialization diagnostics.
  //
  typedef ::xsd::cxx::tree::severity severity;
  typedef ::xsd::cxx::tree::error< char > error;
  typedef ::xsd::cxx::tree::diagnostics< char > diagnostics;

  // Exceptions.
  //
  typedef ::xsd::cxx::tree::exception< char > exception;
  typedef ::xsd::cxx::tree::bounds< char > bounds;
  typedef ::xsd::cxx::tree::duplicate_id< char > duplicate_id;
  typedef ::xsd::cxx::tree::parsing< char > parsing;
  typedef ::xsd::cxx::tree::expected_element< char > expected_element;
  typedef ::xsd::cxx::tree::unexpected_element< char > unexpected_element;
  typedef ::xsd::cxx::tree::expected_attribute< char > expected_attribute;
  typedef ::xsd::cxx::tree::unexpected_enumerator< char > unexpected_enumerator;
  typedef ::xsd::cxx::tree::expected_text_content< char > expected_text_content;
  typedef ::xsd::cxx::tree::no_prefix_mapping< char > no_prefix_mapping;
  typedef ::xsd::cxx::tree::serialization< char > serialization;

  // Error handler callback interface.
  //
  typedef ::xsd::cxx::xml::error_handler< char > error_handler;

  // DOM interaction.
  //
  namespace dom
  {
    // Automatic pointer for DOMDocument.
    //
    using ::xsd::cxx::xml::dom::unique_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
    // DOM user data key for back pointers to tree nodes.
    //
    const XMLCh* const tree_node_key = ::xsd::cxx::tree::user_data_keys::node;
#endif
  }
}

// Forward declarations.
//
class vectorType;
class simulation;
class parameters;
class clusters;
class particle;
class disc;
class cuboid;

#include <memory>    // ::std::unique_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search
#include <utility>   // std::move

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class vectorType: public ::xml_schema::type
{
  public:
  // x
  //
  typedef ::xml_schema::double_ x_type;
  typedef ::xsd::cxx::tree::traits< x_type, char, ::xsd::cxx::tree::schema_type::double_ > x_traits;

  const x_type&
  x () const;

  x_type&
  x ();

  void
  x (const x_type& x);

  // y
  //
  typedef ::xml_schema::double_ y_type;
  typedef ::xsd::cxx::tree::traits< y_type, char, ::xsd::cxx::tree::schema_type::double_ > y_traits;

  const y_type&
  y () const;

  y_type&
  y ();

  void
  y (const y_type& x);

  // z
  //
  typedef ::xml_schema::double_ z_type;
  typedef ::xsd::cxx::tree::traits< z_type, char, ::xsd::cxx::tree::schema_type::double_ > z_traits;

  const z_type&
  z () const;

  z_type&
  z ();

  void
  z (const z_type& x);

  // Constructors.
  //
  vectorType (const x_type&,
              const y_type&,
              const z_type&);

  vectorType (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  vectorType (const vectorType& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual vectorType*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  vectorType&
  operator= (const vectorType& x);

  virtual 
  ~vectorType ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< x_type > x_;
  ::xsd::cxx::tree::one< y_type > y_;
  ::xsd::cxx::tree::one< z_type > z_;
};

class simulation: public ::xml_schema::type
{
  public:
  // parameters
  //
  typedef ::parameters parameters_type;
  typedef ::xsd::cxx::tree::optional< parameters_type > parameters_optional;
  typedef ::xsd::cxx::tree::traits< parameters_type, char > parameters_traits;

  const parameters_optional&
  parameters () const;

  parameters_optional&
  parameters ();

  void
  parameters (const parameters_type& x);

  void
  parameters (const parameters_optional& x);

  void
  parameters (::std::unique_ptr< parameters_type > p);

  // clusters
  //
  typedef ::clusters clusters_type;
  typedef ::xsd::cxx::tree::traits< clusters_type, char > clusters_traits;

  const clusters_type&
  clusters () const;

  clusters_type&
  clusters ();

  void
  clusters (const clusters_type& x);

  void
  clusters (::std::unique_ptr< clusters_type > p);

  // Constructors.
  //
  simulation (const clusters_type&);

  simulation (::std::unique_ptr< clusters_type >);

  simulation (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  simulation (const simulation& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual simulation*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  simulation&
  operator= (const simulation& x);

  virtual 
  ~simulation ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  parameters_optional parameters_;
  ::xsd::cxx::tree::one< clusters_type > clusters_;
};

class parameters: public ::xml_schema::type
{
  public:
  // delta_t
  //
  typedef ::xml_schema::double_ delta_t_type;
  typedef ::xsd::cxx::tree::optional< delta_t_type > delta_t_optional;
  typedef ::xsd::cxx::tree::traits< delta_t_type, char, ::xsd::cxx::tree::schema_type::double_ > delta_t_traits;

  const delta_t_optional&
  delta_t () const;

  delta_t_optional&
  delta_t ();

  void
  delta_t (const delta_t_type& x);

  void
  delta_t (const delta_t_optional& x);

  // t_end
  //
  typedef ::xml_schema::double_ t_end_type;
  typedef ::xsd::cxx::tree::optional< t_end_type > t_end_optional;
  typedef ::xsd::cxx::tree::traits< t_end_type, char, ::xsd::cxx::tree::schema_type::double_ > t_end_traits;

  const t_end_optional&
  t_end () const;

  t_end_optional&
  t_end ();

  void
  t_end (const t_end_type& x);

  void
  t_end (const t_end_optional& x);

  // epsilon
  //
  typedef ::xml_schema::double_ epsilon_type;
  typedef ::xsd::cxx::tree::optional< epsilon_type > epsilon_optional;
  typedef ::xsd::cxx::tree::traits< epsilon_type, char, ::xsd::cxx::tree::schema_type::double_ > epsilon_traits;

  const epsilon_optional&
  epsilon () const;

  epsilon_optional&
  epsilon ();

  void
  epsilon (const epsilon_type& x);

  void
  epsilon (const epsilon_optional& x);

  // sigma
  //
  typedef ::xml_schema::double_ sigma_type;
  typedef ::xsd::cxx::tree::optional< sigma_type > sigma_optional;
  typedef ::xsd::cxx::tree::traits< sigma_type, char, ::xsd::cxx::tree::schema_type::double_ > sigma_traits;

  const sigma_optional&
  sigma () const;

  sigma_optional&
  sigma ();

  void
  sigma (const sigma_type& x);

  void
  sigma (const sigma_optional& x);

  // t_start
  //
  typedef ::xml_schema::double_ t_start_type;
  typedef ::xsd::cxx::tree::optional< t_start_type > t_start_optional;
  typedef ::xsd::cxx::tree::traits< t_start_type, char, ::xsd::cxx::tree::schema_type::double_ > t_start_traits;

  const t_start_optional&
  t_start () const;

  t_start_optional&
  t_start ();

  void
  t_start (const t_start_type& x);

  void
  t_start (const t_start_optional& x);

  // Constructors.
  //
  parameters ();

  parameters (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  parameters (const parameters& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual parameters*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  parameters&
  operator= (const parameters& x);

  virtual 
  ~parameters ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  delta_t_optional delta_t_;
  t_end_optional t_end_;
  epsilon_optional epsilon_;
  sigma_optional sigma_;
  t_start_optional t_start_;
};

class clusters: public ::xml_schema::type
{
  public:
  // particle
  //
  typedef ::particle particle_type;
  typedef ::xsd::cxx::tree::sequence< particle_type > particle_sequence;
  typedef particle_sequence::iterator particle_iterator;
  typedef particle_sequence::const_iterator particle_const_iterator;
  typedef ::xsd::cxx::tree::traits< particle_type, char > particle_traits;

  const particle_sequence&
  particle () const;

  particle_sequence&
  particle ();

  void
  particle (const particle_sequence& s);

  // disc
  //
  typedef ::disc disc_type;
  typedef ::xsd::cxx::tree::sequence< disc_type > disc_sequence;
  typedef disc_sequence::iterator disc_iterator;
  typedef disc_sequence::const_iterator disc_const_iterator;
  typedef ::xsd::cxx::tree::traits< disc_type, char > disc_traits;

  const disc_sequence&
  disc () const;

  disc_sequence&
  disc ();

  void
  disc (const disc_sequence& s);

  // cuboid
  //
  typedef ::cuboid cuboid_type;
  typedef ::xsd::cxx::tree::sequence< cuboid_type > cuboid_sequence;
  typedef cuboid_sequence::iterator cuboid_iterator;
  typedef cuboid_sequence::const_iterator cuboid_const_iterator;
  typedef ::xsd::cxx::tree::traits< cuboid_type, char > cuboid_traits;

  const cuboid_sequence&
  cuboid () const;

  cuboid_sequence&
  cuboid ();

  void
  cuboid (const cuboid_sequence& s);

  // Constructors.
  //
  clusters ();

  clusters (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  clusters (const clusters& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual clusters*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  clusters&
  operator= (const clusters& x);

  virtual 
  ~clusters ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  particle_sequence particle_;
  disc_sequence disc_;
  cuboid_sequence cuboid_;
};

class particle: public ::xml_schema::type
{
  public:
  // coordinate
  //
  typedef ::vectorType coordinate_type;
  typedef ::xsd::cxx::tree::traits< coordinate_type, char > coordinate_traits;

  const coordinate_type&
  coordinate () const;

  coordinate_type&
  coordinate ();

  void
  coordinate (const coordinate_type& x);

  void
  coordinate (::std::unique_ptr< coordinate_type > p);

  // velocity
  //
  typedef ::vectorType velocity_type;
  typedef ::xsd::cxx::tree::traits< velocity_type, char > velocity_traits;

  const velocity_type&
  velocity () const;

  velocity_type&
  velocity ();

  void
  velocity (const velocity_type& x);

  void
  velocity (::std::unique_ptr< velocity_type > p);

  // mass
  //
  typedef ::xml_schema::double_ mass_type;
  typedef ::xsd::cxx::tree::traits< mass_type, char, ::xsd::cxx::tree::schema_type::double_ > mass_traits;

  const mass_type&
  mass () const;

  mass_type&
  mass ();

  void
  mass (const mass_type& x);

  // Constructors.
  //
  particle (const coordinate_type&,
            const velocity_type&,
            const mass_type&);

  particle (::std::unique_ptr< coordinate_type >,
            ::std::unique_ptr< velocity_type >,
            const mass_type&);

  particle (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  particle (const particle& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual particle*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  particle&
  operator= (const particle& x);

  virtual 
  ~particle ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< coordinate_type > coordinate_;
  ::xsd::cxx::tree::one< velocity_type > velocity_;
  ::xsd::cxx::tree::one< mass_type > mass_;
};

class disc: public ::xml_schema::type
{
  public:
  // center
  //
  typedef ::vectorType center_type;
  typedef ::xsd::cxx::tree::traits< center_type, char > center_traits;

  const center_type&
  center () const;

  center_type&
  center ();

  void
  center (const center_type& x);

  void
  center (::std::unique_ptr< center_type > p);

  // velocity
  //
  typedef ::vectorType velocity_type;
  typedef ::xsd::cxx::tree::traits< velocity_type, char > velocity_traits;

  const velocity_type&
  velocity () const;

  velocity_type&
  velocity ();

  void
  velocity (const velocity_type& x);

  void
  velocity (::std::unique_ptr< velocity_type > p);

  // mass
  //
  typedef ::xml_schema::double_ mass_type;
  typedef ::xsd::cxx::tree::traits< mass_type, char, ::xsd::cxx::tree::schema_type::double_ > mass_traits;

  const mass_type&
  mass () const;

  mass_type&
  mass ();

  void
  mass (const mass_type& x);

  // meshWidth
  //
  typedef ::xml_schema::double_ meshWidth_type;
  typedef ::xsd::cxx::tree::traits< meshWidth_type, char, ::xsd::cxx::tree::schema_type::double_ > meshWidth_traits;

  const meshWidth_type&
  meshWidth () const;

  meshWidth_type&
  meshWidth ();

  void
  meshWidth (const meshWidth_type& x);

  // radius
  //
  typedef ::xml_schema::double_ radius_type;
  typedef ::xsd::cxx::tree::traits< radius_type, char, ::xsd::cxx::tree::schema_type::double_ > radius_traits;

  const radius_type&
  radius () const;

  radius_type&
  radius ();

  void
  radius (const radius_type& x);

  // Constructors.
  //
  disc (const center_type&,
        const velocity_type&,
        const mass_type&,
        const meshWidth_type&,
        const radius_type&);

  disc (::std::unique_ptr< center_type >,
        ::std::unique_ptr< velocity_type >,
        const mass_type&,
        const meshWidth_type&,
        const radius_type&);

  disc (const ::xercesc::DOMElement& e,
        ::xml_schema::flags f = 0,
        ::xml_schema::container* c = 0);

  disc (const disc& x,
        ::xml_schema::flags f = 0,
        ::xml_schema::container* c = 0);

  virtual disc*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  disc&
  operator= (const disc& x);

  virtual 
  ~disc ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< center_type > center_;
  ::xsd::cxx::tree::one< velocity_type > velocity_;
  ::xsd::cxx::tree::one< mass_type > mass_;
  ::xsd::cxx::tree::one< meshWidth_type > meshWidth_;
  ::xsd::cxx::tree::one< radius_type > radius_;
};

class cuboid: public ::xml_schema::type
{
  public:
  // cornerCoordinates
  //
  typedef ::vectorType cornerCoordinates_type;
  typedef ::xsd::cxx::tree::traits< cornerCoordinates_type, char > cornerCoordinates_traits;

  const cornerCoordinates_type&
  cornerCoordinates () const;

  cornerCoordinates_type&
  cornerCoordinates ();

  void
  cornerCoordinates (const cornerCoordinates_type& x);

  void
  cornerCoordinates (::std::unique_ptr< cornerCoordinates_type > p);

  // dimensions
  //
  typedef ::vectorType dimensions_type;
  typedef ::xsd::cxx::tree::traits< dimensions_type, char > dimensions_traits;

  const dimensions_type&
  dimensions () const;

  dimensions_type&
  dimensions ();

  void
  dimensions (const dimensions_type& x);

  void
  dimensions (::std::unique_ptr< dimensions_type > p);

  // velocity
  //
  typedef ::vectorType velocity_type;
  typedef ::xsd::cxx::tree::traits< velocity_type, char > velocity_traits;

  const velocity_type&
  velocity () const;

  velocity_type&
  velocity ();

  void
  velocity (const velocity_type& x);

  void
  velocity (::std::unique_ptr< velocity_type > p);

  // meshWidth
  //
  typedef ::xml_schema::double_ meshWidth_type;
  typedef ::xsd::cxx::tree::traits< meshWidth_type, char, ::xsd::cxx::tree::schema_type::double_ > meshWidth_traits;

  const meshWidth_type&
  meshWidth () const;

  meshWidth_type&
  meshWidth ();

  void
  meshWidth (const meshWidth_type& x);

  // mass
  //
  typedef ::xml_schema::double_ mass_type;
  typedef ::xsd::cxx::tree::traits< mass_type, char, ::xsd::cxx::tree::schema_type::double_ > mass_traits;

  const mass_type&
  mass () const;

  mass_type&
  mass ();

  void
  mass (const mass_type& x);

  // brownianMotion
  //
  typedef ::xml_schema::double_ brownianMotion_type;
  typedef ::xsd::cxx::tree::traits< brownianMotion_type, char, ::xsd::cxx::tree::schema_type::double_ > brownianMotion_traits;

  const brownianMotion_type&
  brownianMotion () const;

  brownianMotion_type&
  brownianMotion ();

  void
  brownianMotion (const brownianMotion_type& x);

  // Constructors.
  //
  cuboid (const cornerCoordinates_type&,
          const dimensions_type&,
          const velocity_type&,
          const meshWidth_type&,
          const mass_type&,
          const brownianMotion_type&);

  cuboid (::std::unique_ptr< cornerCoordinates_type >,
          ::std::unique_ptr< dimensions_type >,
          ::std::unique_ptr< velocity_type >,
          const meshWidth_type&,
          const mass_type&,
          const brownianMotion_type&);

  cuboid (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  cuboid (const cuboid& x,
          ::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0);

  virtual cuboid*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  cuboid&
  operator= (const cuboid& x);

  virtual 
  ~cuboid ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< cornerCoordinates_type > cornerCoordinates_;
  ::xsd::cxx::tree::one< dimensions_type > dimensions_;
  ::xsd::cxx::tree::one< velocity_type > velocity_;
  ::xsd::cxx::tree::one< meshWidth_type > meshWidth_;
  ::xsd::cxx::tree::one< mass_type > mass_;
  ::xsd::cxx::tree::one< brownianMotion_type > brownianMotion_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

// Parse a URI or a local file.
//

::std::unique_ptr< ::simulation >
simulation_ (const ::std::string& uri,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (const ::std::string& uri,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (const ::std::string& uri,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse std::istream.
//

::std::unique_ptr< ::simulation >
simulation_ (::std::istream& is,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::std::istream& is,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::std::istream& is,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::std::istream& is,
             const ::std::string& id,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::std::istream& is,
             const ::std::string& id,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::std::istream& is,
             const ::std::string& id,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::InputSource.
//

::std::unique_ptr< ::simulation >
simulation_ (::xercesc::InputSource& is,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::xercesc::InputSource& is,
             ::xml_schema::error_handler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::xercesc::InputSource& is,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::DOMDocument.
//

::std::unique_ptr< ::simulation >
simulation_ (const ::xercesc::DOMDocument& d,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::unique_ptr< ::simulation >
simulation_ (::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d,
             ::xml_schema::flags f = 0,
             const ::xml_schema::properties& p = ::xml_schema::properties ());

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

void
operator<< (::xercesc::DOMElement&, const vectorType&);

// Serialize to std::ostream.
//

void
simulation_ (::std::ostream& os,
             const ::simulation& x, 
             const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::flags f = 0);

void
simulation_ (::std::ostream& os,
             const ::simulation& x, 
             ::xml_schema::error_handler& eh,
             const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::flags f = 0);

void
simulation_ (::std::ostream& os,
             const ::simulation& x, 
             ::xercesc::DOMErrorHandler& eh,
             const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::flags f = 0);

// Serialize to xercesc::XMLFormatTarget.
//

void
simulation_ (::xercesc::XMLFormatTarget& ft,
             const ::simulation& x, 
             const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::flags f = 0);

void
simulation_ (::xercesc::XMLFormatTarget& ft,
             const ::simulation& x, 
             ::xml_schema::error_handler& eh,
             const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::flags f = 0);

void
simulation_ (::xercesc::XMLFormatTarget& ft,
             const ::simulation& x, 
             ::xercesc::DOMErrorHandler& eh,
             const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::flags f = 0);

// Serialize to an existing xercesc::DOMDocument.
//

void
simulation_ (::xercesc::DOMDocument& d,
             const ::simulation& x,
             ::xml_schema::flags f = 0);

// Serialize to a new xercesc::DOMDocument.
//

::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument >
simulation_ (const ::simulation& x, 
             const ::xml_schema::namespace_infomap& m = ::xml_schema::namespace_infomap (),
             ::xml_schema::flags f = 0);

void
operator<< (::xercesc::DOMElement&, const simulation&);

void
operator<< (::xercesc::DOMElement&, const parameters&);

void
operator<< (::xercesc::DOMElement&, const clusters&);

void
operator<< (::xercesc::DOMElement&, const particle&);

void
operator<< (::xercesc::DOMElement&, const disc&);

void
operator<< (::xercesc::DOMElement&, const cuboid&);

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // MOL_SIM_SRC_INPUT_READER_SIM_PARSER_SIMULATION_H
