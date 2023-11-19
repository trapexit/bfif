#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "CLI11.hpp"
#include "fmt.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "version.hpp"

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <vector>

namespace fs = std::filesystem;

namespace l
{
  static
  std::vector<uint8_t>
  read_file(fs::path const filepath_)
  {
    FILE *f;
    size_t end;
    size_t rv;
    std::vector<uint8_t> data;

    f = fopen(filepath_.string().c_str(),"rb");

    fseek(f,0,SEEK_END);
    end = ftell(f);

    data.resize(end);

    fseek(f,0,SEEK_SET);
    rv = fread(&data[0],1,end,f);
    if(rv != end)
      throw std::runtime_error("Failed to read file");

    fclose(f);

    return data;
  }

  void
  export_images(fs::path const input_filepath_,
                fs::path const output_dirpath_,
                bool const     ignore_false_positives_)
  {
    size_t count;
    std::vector<uint8_t> data;
    fs::path output_filepath;

    fmt::print("Reading {} into memory\n",input_filepath_);
    data = l::read_file(input_filepath_);

    fmt::print("Found images will be written to {}\n",output_dirpath_);

    count = 0;
    for(size_t i = 0; i < data.size(); i++)
      {
        int w;
        int h;
        int n;
        int rv;
        stbi_uc *img;
        fs::path output_filename;

        img = stbi_load_from_memory(&data[i],
                                    data.size() - i,
                                    &w,
                                    &h,
                                    &n,
                                    4);
        if(img == NULL)
          continue;

        if(ignore_false_positives_ && ((w > 4096) || (h > 4096)))
          {
            fmt::print("Found at offset {}: "
                       "x = {}, y = {} "
                       "- seems like a false positive - skipping\n",
                       i, w, h);
            stbi_image_free(img);
            continue;
          }

        output_filename = fmt::format("{}_{:03}_{}_{}x{}.png",
                                      input_filepath_.filename(),
                                      count,
                                      i,
                                      w,
                                      h);

        fmt::print("Found at offset {}: "
                   "x = {}, y = {} - writing {}\n",
                   i, w, h, output_filename);

        output_filepath = output_dirpath_ / output_filename;
        rv = stbi_write_png(output_filepath.string().c_str(),w,h,4,img,w*4);
        if(rv == 0)
          {
            std::string err;

            err = fmt::format("Error writing file {}",output_filepath);

            throw std::system_error(errno,std::system_category(),err);
          }

        stbi_image_free(img);

        count++;
      }
  }
}

int
main(int    argc_,
     char **argv_)
{
  std::string desc;
  fs::path input_filepath;
  fs::path output_dirpath;
  bool ignore_false_positives;
  CLI::App app;

  desc = fmt::format("bfif: brute force image finder (v{}.{}.{})",
                     VERSION_MAJOR,
                     VERSION_MINOR,
                     VERSION_PATCH);
  app.description(desc);

  app.add_option("filepath",input_filepath)
    ->description("Path to input file")
    ->type_name("PATH")
    ->check(CLI::ExistingFile)
    ->required();
  app.add_option("-o,--output-dirpath",output_dirpath)
    ->description("Path to directory to write found images (default: input file directory)")
    ->type_name("PATH")
    ->check(CLI::ExistingDirectory)
    ->take_last();
  app.add_option("-i,--ignore-false-positives",ignore_false_positives)
    ->description("Ignore suspected false positives")
    ->default_val(true)
    ->default_str("true")
    ->take_last();

  try
    {
      app.parse(argc_,argv_);

      if(output_dirpath.empty())
        output_dirpath = input_filepath.parent_path();

      l::export_images(input_filepath,
                       output_dirpath,
                       ignore_false_positives);
    }
  catch(const CLI::ParseError &e_)
    {
      return app.exit(e_);
    }
  catch(const std::system_error &e_)
    {
      fmt::print("{}\n",e_.what());
    }
  catch(const std::runtime_error &e_)
    {
      fmt::print("{}\n",e_.what());
    }

  return 0;
}
