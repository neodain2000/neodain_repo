// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Neodain.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Neodain_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Neodain_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Neodain_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Neodain_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[4]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Neodain_2eproto;
class ACTIVE_NEODAIN_INFO;
class ACTIVE_NEODAIN_INFODefaultTypeInternal;
extern ACTIVE_NEODAIN_INFODefaultTypeInternal _ACTIVE_NEODAIN_INFO_default_instance_;
class NEODAIN_EVENT;
class NEODAIN_EVENTDefaultTypeInternal;
extern NEODAIN_EVENTDefaultTypeInternal _NEODAIN_EVENT_default_instance_;
class NEODAIN_EVENT_RESP;
class NEODAIN_EVENT_RESPDefaultTypeInternal;
extern NEODAIN_EVENT_RESPDefaultTypeInternal _NEODAIN_EVENT_RESP_default_instance_;
class NULL_MSG;
class NULL_MSGDefaultTypeInternal;
extern NULL_MSGDefaultTypeInternal _NULL_MSG_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::ACTIVE_NEODAIN_INFO* Arena::CreateMaybeMessage<::ACTIVE_NEODAIN_INFO>(Arena*);
template<> ::NEODAIN_EVENT* Arena::CreateMaybeMessage<::NEODAIN_EVENT>(Arena*);
template<> ::NEODAIN_EVENT_RESP* Arena::CreateMaybeMessage<::NEODAIN_EVENT_RESP>(Arena*);
template<> ::NULL_MSG* Arena::CreateMaybeMessage<::NULL_MSG>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class NEODAIN_EVENT PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:NEODAIN_EVENT) */ {
 public:
  inline NEODAIN_EVENT() : NEODAIN_EVENT(nullptr) {};
  virtual ~NEODAIN_EVENT();

  NEODAIN_EVENT(const NEODAIN_EVENT& from);
  NEODAIN_EVENT(NEODAIN_EVENT&& from) noexcept
    : NEODAIN_EVENT() {
    *this = ::std::move(from);
  }

  inline NEODAIN_EVENT& operator=(const NEODAIN_EVENT& from) {
    CopyFrom(from);
    return *this;
  }
  inline NEODAIN_EVENT& operator=(NEODAIN_EVENT&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const NEODAIN_EVENT& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NEODAIN_EVENT* internal_default_instance() {
    return reinterpret_cast<const NEODAIN_EVENT*>(
               &_NEODAIN_EVENT_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(NEODAIN_EVENT& a, NEODAIN_EVENT& b) {
    a.Swap(&b);
  }
  inline void Swap(NEODAIN_EVENT* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NEODAIN_EVENT* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NEODAIN_EVENT* New() const final {
    return CreateMaybeMessage<NEODAIN_EVENT>(nullptr);
  }

  NEODAIN_EVENT* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NEODAIN_EVENT>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const NEODAIN_EVENT& from);
  void MergeFrom(const NEODAIN_EVENT& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NEODAIN_EVENT* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "NEODAIN_EVENT";
  }
  protected:
  explicit NEODAIN_EVENT(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Neodain_2eproto);
    return ::descriptor_table_Neodain_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIndexFieldNumber = 1,
    kMsgCntFieldNumber = 2,
  };
  // optional uint32 index = 1;
  bool has_index() const;
  private:
  bool _internal_has_index() const;
  public:
  void clear_index();
  ::PROTOBUF_NAMESPACE_ID::uint32 index() const;
  void set_index(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_index() const;
  void _internal_set_index(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // optional uint32 msgCnt = 2;
  bool has_msgcnt() const;
  private:
  bool _internal_has_msgcnt() const;
  public:
  void clear_msgcnt();
  ::PROTOBUF_NAMESPACE_ID::uint32 msgcnt() const;
  void set_msgcnt(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_msgcnt() const;
  void _internal_set_msgcnt(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:NEODAIN_EVENT)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::uint32 index_;
  ::PROTOBUF_NAMESPACE_ID::uint32 msgcnt_;
  friend struct ::TableStruct_Neodain_2eproto;
};
// -------------------------------------------------------------------

class NEODAIN_EVENT_RESP PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:NEODAIN_EVENT_RESP) */ {
 public:
  inline NEODAIN_EVENT_RESP() : NEODAIN_EVENT_RESP(nullptr) {};
  virtual ~NEODAIN_EVENT_RESP();

  NEODAIN_EVENT_RESP(const NEODAIN_EVENT_RESP& from);
  NEODAIN_EVENT_RESP(NEODAIN_EVENT_RESP&& from) noexcept
    : NEODAIN_EVENT_RESP() {
    *this = ::std::move(from);
  }

  inline NEODAIN_EVENT_RESP& operator=(const NEODAIN_EVENT_RESP& from) {
    CopyFrom(from);
    return *this;
  }
  inline NEODAIN_EVENT_RESP& operator=(NEODAIN_EVENT_RESP&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const NEODAIN_EVENT_RESP& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NEODAIN_EVENT_RESP* internal_default_instance() {
    return reinterpret_cast<const NEODAIN_EVENT_RESP*>(
               &_NEODAIN_EVENT_RESP_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(NEODAIN_EVENT_RESP& a, NEODAIN_EVENT_RESP& b) {
    a.Swap(&b);
  }
  inline void Swap(NEODAIN_EVENT_RESP* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NEODAIN_EVENT_RESP* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NEODAIN_EVENT_RESP* New() const final {
    return CreateMaybeMessage<NEODAIN_EVENT_RESP>(nullptr);
  }

  NEODAIN_EVENT_RESP* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NEODAIN_EVENT_RESP>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const NEODAIN_EVENT_RESP& from);
  void MergeFrom(const NEODAIN_EVENT_RESP& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NEODAIN_EVENT_RESP* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "NEODAIN_EVENT_RESP";
  }
  protected:
  explicit NEODAIN_EVENT_RESP(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Neodain_2eproto);
    return ::descriptor_table_Neodain_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kReasonFieldNumber = 2,
    kResultFieldNumber = 1,
  };
  // optional string reason = 2;
  bool has_reason() const;
  private:
  bool _internal_has_reason() const;
  public:
  void clear_reason();
  const std::string& reason() const;
  void set_reason(const std::string& value);
  void set_reason(std::string&& value);
  void set_reason(const char* value);
  void set_reason(const char* value, size_t size);
  std::string* mutable_reason();
  std::string* release_reason();
  void set_allocated_reason(std::string* reason);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_reason();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_reason(
      std::string* reason);
  private:
  const std::string& _internal_reason() const;
  void _internal_set_reason(const std::string& value);
  std::string* _internal_mutable_reason();
  public:

  // optional uint32 result = 1;
  bool has_result() const;
  private:
  bool _internal_has_result() const;
  public:
  void clear_result();
  ::PROTOBUF_NAMESPACE_ID::uint32 result() const;
  void set_result(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_result() const;
  void _internal_set_result(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:NEODAIN_EVENT_RESP)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr reason_;
  ::PROTOBUF_NAMESPACE_ID::uint32 result_;
  friend struct ::TableStruct_Neodain_2eproto;
};
// -------------------------------------------------------------------

class ACTIVE_NEODAIN_INFO PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ACTIVE_NEODAIN_INFO) */ {
 public:
  inline ACTIVE_NEODAIN_INFO() : ACTIVE_NEODAIN_INFO(nullptr) {};
  virtual ~ACTIVE_NEODAIN_INFO();

  ACTIVE_NEODAIN_INFO(const ACTIVE_NEODAIN_INFO& from);
  ACTIVE_NEODAIN_INFO(ACTIVE_NEODAIN_INFO&& from) noexcept
    : ACTIVE_NEODAIN_INFO() {
    *this = ::std::move(from);
  }

  inline ACTIVE_NEODAIN_INFO& operator=(const ACTIVE_NEODAIN_INFO& from) {
    CopyFrom(from);
    return *this;
  }
  inline ACTIVE_NEODAIN_INFO& operator=(ACTIVE_NEODAIN_INFO&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const ACTIVE_NEODAIN_INFO& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ACTIVE_NEODAIN_INFO* internal_default_instance() {
    return reinterpret_cast<const ACTIVE_NEODAIN_INFO*>(
               &_ACTIVE_NEODAIN_INFO_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(ACTIVE_NEODAIN_INFO& a, ACTIVE_NEODAIN_INFO& b) {
    a.Swap(&b);
  }
  inline void Swap(ACTIVE_NEODAIN_INFO* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ACTIVE_NEODAIN_INFO* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ACTIVE_NEODAIN_INFO* New() const final {
    return CreateMaybeMessage<ACTIVE_NEODAIN_INFO>(nullptr);
  }

  ACTIVE_NEODAIN_INFO* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ACTIVE_NEODAIN_INFO>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ACTIVE_NEODAIN_INFO& from);
  void MergeFrom(const ACTIVE_NEODAIN_INFO& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ACTIVE_NEODAIN_INFO* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "ACTIVE_NEODAIN_INFO";
  }
  protected:
  explicit ACTIVE_NEODAIN_INFO(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Neodain_2eproto);
    return ::descriptor_table_Neodain_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kTraceCmfListFieldNumber = 1,
  };
  // repeated .NEODAIN_EVENT traceCmfList = 1;
  int tracecmflist_size() const;
  private:
  int _internal_tracecmflist_size() const;
  public:
  void clear_tracecmflist();
  ::NEODAIN_EVENT* mutable_tracecmflist(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::NEODAIN_EVENT >*
      mutable_tracecmflist();
  private:
  const ::NEODAIN_EVENT& _internal_tracecmflist(int index) const;
  ::NEODAIN_EVENT* _internal_add_tracecmflist();
  public:
  const ::NEODAIN_EVENT& tracecmflist(int index) const;
  ::NEODAIN_EVENT* add_tracecmflist();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::NEODAIN_EVENT >&
      tracecmflist() const;

  // @@protoc_insertion_point(class_scope:ACTIVE_NEODAIN_INFO)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::NEODAIN_EVENT > tracecmflist_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Neodain_2eproto;
};
// -------------------------------------------------------------------

class NULL_MSG PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:NULL_MSG) */ {
 public:
  inline NULL_MSG() : NULL_MSG(nullptr) {};
  virtual ~NULL_MSG();

  NULL_MSG(const NULL_MSG& from);
  NULL_MSG(NULL_MSG&& from) noexcept
    : NULL_MSG() {
    *this = ::std::move(from);
  }

  inline NULL_MSG& operator=(const NULL_MSG& from) {
    CopyFrom(from);
    return *this;
  }
  inline NULL_MSG& operator=(NULL_MSG&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const NULL_MSG& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NULL_MSG* internal_default_instance() {
    return reinterpret_cast<const NULL_MSG*>(
               &_NULL_MSG_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(NULL_MSG& a, NULL_MSG& b) {
    a.Swap(&b);
  }
  inline void Swap(NULL_MSG* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NULL_MSG* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NULL_MSG* New() const final {
    return CreateMaybeMessage<NULL_MSG>(nullptr);
  }

  NULL_MSG* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NULL_MSG>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const NULL_MSG& from);
  void MergeFrom(const NULL_MSG& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NULL_MSG* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "NULL_MSG";
  }
  protected:
  explicit NULL_MSG(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Neodain_2eproto);
    return ::descriptor_table_Neodain_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:NULL_MSG)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Neodain_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// NEODAIN_EVENT

// optional uint32 index = 1;
inline bool NEODAIN_EVENT::_internal_has_index() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool NEODAIN_EVENT::has_index() const {
  return _internal_has_index();
}
inline void NEODAIN_EVENT::clear_index() {
  index_ = 0u;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NEODAIN_EVENT::_internal_index() const {
  return index_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NEODAIN_EVENT::index() const {
  // @@protoc_insertion_point(field_get:NEODAIN_EVENT.index)
  return _internal_index();
}
inline void NEODAIN_EVENT::_internal_set_index(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000001u;
  index_ = value;
}
inline void NEODAIN_EVENT::set_index(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_index(value);
  // @@protoc_insertion_point(field_set:NEODAIN_EVENT.index)
}

// optional uint32 msgCnt = 2;
inline bool NEODAIN_EVENT::_internal_has_msgcnt() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool NEODAIN_EVENT::has_msgcnt() const {
  return _internal_has_msgcnt();
}
inline void NEODAIN_EVENT::clear_msgcnt() {
  msgcnt_ = 0u;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NEODAIN_EVENT::_internal_msgcnt() const {
  return msgcnt_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NEODAIN_EVENT::msgcnt() const {
  // @@protoc_insertion_point(field_get:NEODAIN_EVENT.msgCnt)
  return _internal_msgcnt();
}
inline void NEODAIN_EVENT::_internal_set_msgcnt(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000002u;
  msgcnt_ = value;
}
inline void NEODAIN_EVENT::set_msgcnt(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_msgcnt(value);
  // @@protoc_insertion_point(field_set:NEODAIN_EVENT.msgCnt)
}

// -------------------------------------------------------------------

// NEODAIN_EVENT_RESP

// optional uint32 result = 1;
inline bool NEODAIN_EVENT_RESP::_internal_has_result() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool NEODAIN_EVENT_RESP::has_result() const {
  return _internal_has_result();
}
inline void NEODAIN_EVENT_RESP::clear_result() {
  result_ = 0u;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NEODAIN_EVENT_RESP::_internal_result() const {
  return result_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 NEODAIN_EVENT_RESP::result() const {
  // @@protoc_insertion_point(field_get:NEODAIN_EVENT_RESP.result)
  return _internal_result();
}
inline void NEODAIN_EVENT_RESP::_internal_set_result(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _has_bits_[0] |= 0x00000002u;
  result_ = value;
}
inline void NEODAIN_EVENT_RESP::set_result(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_result(value);
  // @@protoc_insertion_point(field_set:NEODAIN_EVENT_RESP.result)
}

// optional string reason = 2;
inline bool NEODAIN_EVENT_RESP::_internal_has_reason() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool NEODAIN_EVENT_RESP::has_reason() const {
  return _internal_has_reason();
}
inline void NEODAIN_EVENT_RESP::clear_reason() {
  reason_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& NEODAIN_EVENT_RESP::reason() const {
  // @@protoc_insertion_point(field_get:NEODAIN_EVENT_RESP.reason)
  return _internal_reason();
}
inline void NEODAIN_EVENT_RESP::set_reason(const std::string& value) {
  _internal_set_reason(value);
  // @@protoc_insertion_point(field_set:NEODAIN_EVENT_RESP.reason)
}
inline std::string* NEODAIN_EVENT_RESP::mutable_reason() {
  // @@protoc_insertion_point(field_mutable:NEODAIN_EVENT_RESP.reason)
  return _internal_mutable_reason();
}
inline const std::string& NEODAIN_EVENT_RESP::_internal_reason() const {
  return reason_.Get();
}
inline void NEODAIN_EVENT_RESP::_internal_set_reason(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  reason_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void NEODAIN_EVENT_RESP::set_reason(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  reason_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:NEODAIN_EVENT_RESP.reason)
}
inline void NEODAIN_EVENT_RESP::set_reason(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  reason_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:NEODAIN_EVENT_RESP.reason)
}
inline void NEODAIN_EVENT_RESP::set_reason(const char* value,
    size_t size) {
  _has_bits_[0] |= 0x00000001u;
  reason_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:NEODAIN_EVENT_RESP.reason)
}
inline std::string* NEODAIN_EVENT_RESP::_internal_mutable_reason() {
  _has_bits_[0] |= 0x00000001u;
  return reason_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* NEODAIN_EVENT_RESP::release_reason() {
  // @@protoc_insertion_point(field_release:NEODAIN_EVENT_RESP.reason)
  if (!_internal_has_reason()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return reason_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void NEODAIN_EVENT_RESP::set_allocated_reason(std::string* reason) {
  if (reason != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  reason_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), reason,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:NEODAIN_EVENT_RESP.reason)
}
inline std::string* NEODAIN_EVENT_RESP::unsafe_arena_release_reason() {
  // @@protoc_insertion_point(field_unsafe_arena_release:NEODAIN_EVENT_RESP.reason)
  GOOGLE_DCHECK(GetArena() != nullptr);
  _has_bits_[0] &= ~0x00000001u;
  return reason_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void NEODAIN_EVENT_RESP::unsafe_arena_set_allocated_reason(
    std::string* reason) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (reason != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  reason_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      reason, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:NEODAIN_EVENT_RESP.reason)
}

// -------------------------------------------------------------------

// ACTIVE_NEODAIN_INFO

// repeated .NEODAIN_EVENT traceCmfList = 1;
inline int ACTIVE_NEODAIN_INFO::_internal_tracecmflist_size() const {
  return tracecmflist_.size();
}
inline int ACTIVE_NEODAIN_INFO::tracecmflist_size() const {
  return _internal_tracecmflist_size();
}
inline void ACTIVE_NEODAIN_INFO::clear_tracecmflist() {
  tracecmflist_.Clear();
}
inline ::NEODAIN_EVENT* ACTIVE_NEODAIN_INFO::mutable_tracecmflist(int index) {
  // @@protoc_insertion_point(field_mutable:ACTIVE_NEODAIN_INFO.traceCmfList)
  return tracecmflist_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::NEODAIN_EVENT >*
ACTIVE_NEODAIN_INFO::mutable_tracecmflist() {
  // @@protoc_insertion_point(field_mutable_list:ACTIVE_NEODAIN_INFO.traceCmfList)
  return &tracecmflist_;
}
inline const ::NEODAIN_EVENT& ACTIVE_NEODAIN_INFO::_internal_tracecmflist(int index) const {
  return tracecmflist_.Get(index);
}
inline const ::NEODAIN_EVENT& ACTIVE_NEODAIN_INFO::tracecmflist(int index) const {
  // @@protoc_insertion_point(field_get:ACTIVE_NEODAIN_INFO.traceCmfList)
  return _internal_tracecmflist(index);
}
inline ::NEODAIN_EVENT* ACTIVE_NEODAIN_INFO::_internal_add_tracecmflist() {
  return tracecmflist_.Add();
}
inline ::NEODAIN_EVENT* ACTIVE_NEODAIN_INFO::add_tracecmflist() {
  // @@protoc_insertion_point(field_add:ACTIVE_NEODAIN_INFO.traceCmfList)
  return _internal_add_tracecmflist();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::NEODAIN_EVENT >&
ACTIVE_NEODAIN_INFO::tracecmflist() const {
  // @@protoc_insertion_point(field_list:ACTIVE_NEODAIN_INFO.traceCmfList)
  return tracecmflist_;
}

// -------------------------------------------------------------------

// NULL_MSG

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Neodain_2eproto
