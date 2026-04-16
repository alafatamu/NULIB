This is a full restructure of the TNLIB codebase. 
This has been written to to improve readability and maintainability.

When starting a new project, be sure to add these lines in the following order:

INFOCON cfg = load_config("path/to/config.cfg");

  Load a configuration file into memory. This should contain all path information for the project (in an INFOCON struct). See proggy/config files in include and src directories. 

  See the example config.cfg file for formatting. This data is loaded by token and value, so line order does not matter as long as the format per line is TOKEN = VALUE. Do not add quotes around the values.

  INFOCON structs have default values for all fields. See include/proggy/config.hpp for the default values.

detector texneut;

  Create a detector object. Seeing as this library is primarily for the TexNeut detector, that is the only detector object currently implimented. It contains many maps and vectors related to detector geometry, gains, correction factors, etc. It will have a dedicated breakdown later. For now, just load in your detector maps. 

bool goodmapread = texneut.fill_maps(cfg);

  The fill_maps function will return a bool to indicate whether the maps were found/read successfully. If the maps are not loaded in, the detector object will not be usable, so TNLIB will only be able to read raw data according to the unpacker. Processing requires experimental info.