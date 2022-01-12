#ifndef CONFIG_H_
#define CONFIG_H_

class Config {
  private:
    std::vector< std::string > zNames {};
    std::vector< std::vector < std::string > >
      zValues {};
  public:
    Config();
    Config(std::string filename);
    ~Config();
    void init();
    void read_line(std::string line);
    int name_occurs(std::string name);
    std::vector < std::string > get_strings(
      std::string name
    );
    std::vector < int > get_ints(std::string name);
    std::vector < double > get_floats(
      std::string name);
    std::string get_string(std::string name);
    int get_int(std::string name);
    double get_float(std::string name);
    void read_file(std::string filename);
};

#endif
