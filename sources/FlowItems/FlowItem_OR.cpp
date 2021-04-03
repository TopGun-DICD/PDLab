#include "FlowItem_OR.hpp"

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "LayoutLoader.hpp"
#include "../Helper.hpp"

FlowItem_OR::FlowItem_OR(BasicLogger *logger) : FlowItem(FlowItemType::OR, QString("OR"), logger, LayoutOwnershipMode::make_copy) {
  AddInputPort(PortDataType::layout);
  AddInputPort(PortDataType::layout);
  AddOutputPort(PortDataType::layout);
  titleBgColor = QColor(139, 55, 78);
  //bottomString = QString("KLayout");
}

FlowItem_OR::~FlowItem_OR() {
}

bool FlowItem_OR::DropEventHandler() {
  p_logger->Log("'OR-DROP' was called");
  
  return true;
}

bool FlowItem_OR::ExecuteEventHandler() {
  p_logger->Log("'OR-EXECUTE' was called");

  if (inputPorts[0]->connections.empty() || inputPorts[1]->connections.empty()) {
    p_logger->Error("Can't perform 'OR' operation. One or more ports are not connected.");
    return false;
  }
  for (size_t i = 0; i < p_resultLayout->libraries[0]->elements.size(); ++i) {
    for (size_t j = 0; j < p_resultLayout->libraries[0]->elements[i]->items.size(); ++j) {
      if (p_resultLayout->libraries[0]->elements[i]->items[j]->type == GeometryType::reference)
        continue;
      p_resultLayout->libraries[0]->elements[i]->items[j]->layer = 1;
    }
  }

  Geometry_Polygon   *p_polygon = nullptr;
  Geometry_Path      *p_path    = nullptr;
  Geometry_Text      *p_text    = nullptr;
  Geometry_Box       *p_box     = nullptr;
  Geometry_Reference *p_ref     = nullptr;
  
  for (size_t i = 0; i < inputPorts[1]->GetLayout()->libraries[0]->elements.size(); ++i) {
    Element *p_element = new Element;
    p_element->name = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->name;
    p_element->nested = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->nested;
    p_element->min = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->min;
    p_element->max = inputPorts[1]->GetLayout()->libraries[0]->elements[i]->max;
    p_resultLayout->libraries[0]->elements.push_back(p_element);
    for (size_t j = 0; j < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items.size(); ++j) {
      switch (inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->type) {
        case GeometryType::polygon:
          p_polygon = new Geometry_Polygon;
          p_element->items.push_back(p_polygon);
          p_polygon->type = GeometryType::polygon;
          p_polygon->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties.size(); ++l)
            p_polygon->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords.size(); ++l)
            p_polygon->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords[l]);
          p_polygon->dataType = static_cast<Geometry_Polygon *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->dataType;
          break;
        case GeometryType::box:
          p_box = new Geometry_Box;
          p_element->items.push_back(p_box);
          p_box->type = GeometryType::box;
          p_box->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties.size(); ++l)
            p_box->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords.size(); ++l)
            p_box->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords[l]);
          p_box->boxType = static_cast<Geometry_Box *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->boxType;
          break;
        case GeometryType::path:
          p_path = new Geometry_Path;
          p_element->items.push_back(p_path);
          p_path->type = GeometryType::path;
          p_path->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties.size(); ++l)
            p_path->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords.size(); ++l)
            p_path->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords[l]);
          p_path->dataType = static_cast<Geometry_Path *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->dataType;
          p_path->pathType = static_cast<Geometry_Path *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->pathType;
          p_path->width = static_cast<Geometry_Path *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->width;
          break;
        case GeometryType::text:
          p_text = new Geometry_Text;
          p_element->items.push_back(p_text);
          p_text->type = GeometryType::text;
          p_text->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties.size(); ++l)
            p_text->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords.size(); ++l)
            p_text->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords[l]);
          p_text->textType = static_cast<Geometry_Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->textType;
          p_text->pathType = static_cast<Geometry_Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->pathType;
          p_text->width = static_cast<Geometry_Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->width;
          p_text->flagsPresentation = static_cast<Geometry_Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->flagsPresentation;
          p_text->flagsTransformation = static_cast<Geometry_Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->flagsTransformation;
          p_text->magnification = static_cast<Geometry_Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->magnification;
          p_text->stringValue = static_cast<Geometry_Text *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->stringValue;
          break;
        case GeometryType::reference:
          p_ref = new Geometry_Reference;
          p_element->items.push_back(p_ref);
          p_ref->type = GeometryType::reference;
          p_ref->layer = 1;
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties.size(); ++l)
            p_ref->properties.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->properties[l]);
          for (size_t l = 0; l < inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords.size(); ++l)
            p_ref->coords.push_back(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j]->coords[l]);
          p_ref->name = static_cast<Geometry_Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->name;
          p_ref->referenceTo = static_cast<Geometry_Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->referenceTo;
          p_ref->transformationFlags = static_cast<Geometry_Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->transformationFlags;
          p_ref->magnification = static_cast<Geometry_Reference *>(inputPorts[1]->GetLayout()->libraries[0]->elements[i]->items[j])->magnification;
          break;
      }

    } // !

  }

  p_resultLayout->libraries[0]->layers.clear();
  Layer layer;
  layer.layer = 1;
  layer.name = "OR_layer";
  p_resultLayout->libraries[0]->layers.push_back(layer);

  for (size_t i = 0; i < p_resultLayout->libraries[0]->elements.size(); ++i) {
    for (size_t j = 0; j < p_resultLayout->libraries[0]->elements[i]->items.size(); ++j) {
      if (p_resultLayout->libraries[0]->elements[i]->items[j]->type == GeometryType::reference)
        continue;
      p_resultLayout->libraries[0]->layers[0].items.push_back(p_resultLayout->libraries[0]->elements[i]->items[j]);
    }
  }

  return true;
}

bool FlowItem_OR::OpenResultsEventHandler() {
  p_logger->Log("'OR-OPENRESULTS' was called");

  Helper::GetInstance()->ShowLayout(p_resultLayout);

  return true;
}

bool FlowItem_OR::ResetEventHandler() {
  p_logger->Log("'OR-RESET' was called");

  return true;
}

bool FlowItem_OR::ShowPropertesEventHandler() {
  p_logger->Log("'OR-PROPERTIES' was called");

  return true;
}

QString FlowItem_OR::GetInfoString() {
  return QString("OR info string");
}