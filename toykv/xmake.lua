add_requires("spdlog", {system=false, configs = {header_only = true, fmt_external=true}})
add_requireconfs("spdlog.fmt", {override = true, version = "9.1.0", configs = {header_only = true}})

target("toykv")
  set_kind("shared")
  add_files("$(projectdir)/toykv/**.cpp")
  add_includedirs("$(projectdir)/", { public = true})
  add_packages("fmt", "spdlog")
target_end()
