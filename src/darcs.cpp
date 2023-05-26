#include <darcs_types.h>

namespace DarcsPatch {

    const NilFL_T NilFL;
    const NilRL_T NilRL;

    AddFile::AddFile() {}

    const PATCH_TYPE AddFile::type() const {
        return ADD_FILE;
    }

    const Patch* AddFile::invert() const {
        return new RemoveFile();
    }

    RemoveFile::RemoveFile() {}

    const PATCH_TYPE RemoveFile::type() const {
        return REMOVE_FILE;
    }

    const Patch* RemoveFile::invert() const {
        return new AddFile();
    }

    FileHunk_T* makeHunk_T(const size_t & line, const StringAdapter::CharAdapter& old_line, const StringAdapter::CharAdapter& new_line) {
        return new FileHunk_T(line, old_line, new_line);
    }

    Named_T<Core_FP_T> makeNamedWithType_T(const StringAdapter::CharAdapter & patch_id_unique_label, Patch* patch_type) {
        return Named_T<Core_FP_T>(PatchInfo_T(patch_id_unique_label), NilFL, ToFL(Core_FP_T(patch_type)));
    }

    Named_T<Core_FP_T> makeNamedHunk_T(const StringAdapter::CharAdapter & patch_id_unique_label, const size_t & line, const StringAdapter::CharAdapter & old_line, const StringAdapter::CharAdapter & new_line) {
        return makeNamedWithType_T(patch_id_unique_label, makeHunk_T(line, old_line, new_line));
    }
}