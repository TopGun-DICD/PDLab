#pragma once

#include <string>
#include <vector>

struct Units {
  double  user,
          physical;
};

struct Coord {
  __int32 x,
          y;
};

struct Property {
  __int16     index;
  std::string value;
};

enum class ItemType {
  undefined = 0,
  boundary,
  path,
  text,
  box,
  structRef,
};

enum class DataType {
  noData = 0,
  bitArray,
  WORD,
  DWORD,
  REAL,
  DOUBLE,
  ASCIISTRING,
};

enum class LayoutFileFormat {
  undefined = 0,
  GDSII_bin,
};

struct GeometryItem {
  ItemType                    type;
  __int16                     layer;
  std::vector<Property>       properties;
  std::vector<Coord>          coords;
public:
  GeometryItem(ItemType t);
};

struct GeometryItem_Boundary : public GeometryItem {
  __int16                     dataType;
public:
  GeometryItem_Boundary();
};

struct GeometryItem_Path : public GeometryItem {
  __int16                     dataType,
    pathType;
  __int32                     width;
public:
  GeometryItem_Path();
};

struct GeometryItem_Text : public GeometryItem {
  __int16                     textType,
    flagsPresentation,
    pathType,
    flagsTransformation;
  double                      magnification;
  __int32                     width;
  std::string                 stringValue;
public:
  GeometryItem_Text();
};

struct GeometryItem_Box : public GeometryItem {
  __int16                     boxType;
public:
  GeometryItem_Box();
};

struct Element;

struct GeometryItem_StructureRef : public GeometryItem {
  std::string                 name;
  Element                    *pReference;
  __int16                     transformationFlags;
  double                      magnification;
public:
  GeometryItem_StructureRef();
};

struct Element {
  std::string                 name;
  std::vector<GeometryItem *> items;
public:
  ~Element();
};

struct Layer {
  __int16                     layer;
  std::string                 name;
  std::vector<GeometryItem *> items;
};

struct Library {
  std::string                 name;
  Units                       units;
  std::vector<Element *>      elements;
  std::vector<Layer>          layers;
public:
  ~Library();
};

struct LayoutData {
  std::string                 fileName;
  LayoutFileFormat            fileFormat;
  std::vector<Library *>      libraries;
public:
  LayoutData();
  ~LayoutData();
public:
  bool Read(const std::string &fname);
};
