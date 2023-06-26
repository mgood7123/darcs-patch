#include <darcs_types.h>

namespace DarcsPatch {

    const NilFL_T NilFL;
    const NilRL_T NilRL;

    Patch::~Patch() {}

    void Patch::to_string() const {
        to_stream(std::cout);
    }

    void Patch::to_string() {
        to_stream(std::cout);
    }

    bool Patch::operator == (const DarcsPatch::Patch & other) const {
        return cmp(other) == 0;
    }

    bool Patch::operator != (const DarcsPatch::Patch & other) const {
        return  cmp(other) != 0;
    }

    bool Patch::operator < (const DarcsPatch::Patch & other) const {
        return cmp(other) < 0;
    }

    bool Patch::operator > (const DarcsPatch::Patch & other) const {
        return cmp(other) > 0;
    }

    bool Patch::operator <= (const DarcsPatch::Patch & other) const {
        return cmp(other) <= 0;
    }

    bool Patch::operator >= (const DarcsPatch::Patch & other) const {
        return cmp(other) >= 0;
    }

    int Patch::cmp(const Patch & other) const {
        return StringAdapter::compare_2(type(), other.type());
    }

    std::size_t Patch::hashCode() const noexcept {
        std::size_t hashCode_ = 1;
        hashCode_ = 31 * hashCode_ + std::hash<PATCH_TYPE>()(type());
        return hashCode_;
    }

    const PATCH_TYPE AddFile::type() const {
        return ADD_FILE;
    }

    std::shared_ptr<Patch> AddFile::invert() const {
        return std::static_pointer_cast<Patch>(std::make_shared<RemoveFile>());
    }

    const PATCH_TYPE RemoveFile::type() const {
        return REMOVE_FILE;
    }

    std::shared_ptr<Patch> RemoveFile::invert() const {
        return std::static_pointer_cast<Patch>(std::make_shared<AddFile>());
    }

    std::shared_ptr<Patch> makeAddFile() {
        return std::static_pointer_cast<Patch>(std::make_shared<AddFile>());
    }

    std::shared_ptr<Patch> makeRemoveFile() {
        return std::static_pointer_cast<Patch>(std::make_shared<AddFile>());
    }

    ::std::ostream& Patch::to_stream(::std::ostream& os) const {
        return os << "{ Patch }";
    }

    ::std::ostream& AddFile::to_stream(::std::ostream& os) const {
        return os << "{ AddFile }";
    }

    ::std::ostream& RemoveFile::to_stream(::std::ostream& os) const {
        return os << "{ RemoveFile }";
    }

    std::shared_ptr<Patch> makeHunk_T(const std::size_t & line,const RL<StringAdapter::CharAdapter>& old_lines, const RL<StringAdapter::CharAdapter>& new_lines) {
        return makeHunk<char, StringAdapter::CharAdapter>(line, old_lines, new_lines);
    }
    std::shared_ptr<Patch> makeHunk_T(const size_t & line, const StringAdapter::CharAdapter& old_line, const StringAdapter::CharAdapter& new_line) {
        return makeHunk<char, StringAdapter::CharAdapter>(line, old_line, new_line);
    }

    PatchInfo_T makePatchInfo_T(const StringAdapter::CharAdapter & patch_id_unique_label) {
        return PatchInfo_T("unknown", patch_id_unique_label, "unknown", {});
    }

    Named_T<Core_FP_T> makeNamedWithType_T(const StringAdapter::CharAdapter & patch_id_unique_label, std::shared_ptr<Patch> patch_type) {
        return Named_T<Core_FP_T>(PatchInfo_T("unknown", patch_id_unique_label, "unknown", {}), {}, ToFL(Core_FP_T(patch_type)));
    }

    Named_T<Core_FP_T> makeNamedHunk_T(const StringAdapter::CharAdapter & patch_id_unique_label, const size_t & line, const StringAdapter::CharAdapter & old_line, const StringAdapter::CharAdapter & new_line) {
        return makeNamedWithType_T(patch_id_unique_label, makeHunk_T(line, old_line, new_line));
    }

    std::shared_ptr<Patch> invert(std::shared_ptr<Patch> p) {
        return p->invert();
    }
}

::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Patch & item) {
    return item.to_stream(os);
}

::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::NilFL_T & item) {
    return os << "{ NilFL }";
}

::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::NilRL_T & item) {
    return os << "{ NilRL }";
}
