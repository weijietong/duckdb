//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/function/replacement_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/storage/storage_extension.hpp"

namespace duckdb {

class DatabaseInstance;
class StorageExtension;
struct DBConfig;

struct ReplacementOpenData {
	virtual ~ReplacementOpenData() {
	}

	virtual bool HasStorageExtension() {
		return false;
	}
	virtual unique_ptr<StorageExtension> GetStorageExtension(AttachInfo &info) {
		return nullptr;
	}
};

struct ReplacementOpenStaticData {
	virtual ~ReplacementOpenStaticData() {
	}
};

typedef unique_ptr<ReplacementOpenData> (*replacement_open_pre_t)(DBConfig &config,
                                                                  ReplacementOpenStaticData *static_data);
typedef void (*replacement_open_post_t)(DatabaseInstance &instance, ReplacementOpenData *open_data);

struct ReplacementOpen {
	explicit ReplacementOpen(replacement_open_pre_t pre_func, replacement_open_post_t post_func)
	    : pre_func(pre_func), post_func(post_func), data(nullptr) {
	}

	replacement_open_pre_t pre_func;
	replacement_open_post_t post_func;

	unique_ptr<ReplacementOpenData> data;

	shared_ptr<ReplacementOpenStaticData> static_data;
};

} // namespace duckdb
