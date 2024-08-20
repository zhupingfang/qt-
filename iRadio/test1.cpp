
#include "elements_report_dialog_view_ctrl.h"
#include "layout_common.h"
#include "cmd_keywords.h"

namespace {
const QString EIGHT_SPACE = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
}

using namespace LayoutEng;
using namespace LayoutCmdEngine;

void ElementsReportDialogViewCtrl::InitTypeCountFuncMap()
{
    switchCountFuncMap_.clear();
    switchCountFuncMap_[LayoutEng::GROUPS]           = &ElementsReportDialogViewCtrl::GroupsTypeCount;
    switchCountFuncMap_[LayoutEng::COMPONENTS]       = &ElementsReportDialogViewCtrl::ComponentsTypeCount;
    switchCountFuncMap_[LayoutEng::CELLS]            = &ElementsReportDialogViewCtrl::CellsTypeCount;
    switchCountFuncMap_[LayoutEng::ENETS]            = &ElementsReportDialogViewCtrl::ENetsTypeCount;
    switchCountFuncMap_[LayoutEng::NETS]             = &ElementsReportDialogViewCtrl::NetsTypeCount;
    switchCountFuncMap_[LayoutEng::PINS]             = &ElementsReportDialogViewCtrl::PinsTypeCount;
    switchCountFuncMap_[LayoutEng::VIAS]             = &ElementsReportDialogViewCtrl::ViasTypeCount;
    switchCountFuncMap_[LayoutEng::GUIDELINES]       = &ElementsReportDialogViewCtrl::GuidelinesTypeCount;
    switchCountFuncMap_[LayoutEng::TRACES]           = &ElementsReportDialogViewCtrl::TracesTypeCount;
    switchCountFuncMap_[LayoutEng::LINES]            = &ElementsReportDialogViewCtrl::LinesTypeCount;
    switchCountFuncMap_[LayoutEng::TRACE_SEGS]       = &ElementsReportDialogViewCtrl::TraceSegsTypeCount;
    switchCountFuncMap_[LayoutEng::OTHER_SEGS]       = &ElementsReportDialogViewCtrl::OtherSegsTypeCount;
    switchCountFuncMap_[LayoutEng::AREAS]            = &ElementsReportDialogViewCtrl::AreasTypeCount;
    switchCountFuncMap_[LayoutEng::CUTOUTS_CAVITIES] = &ElementsReportDialogViewCtrl::CutoutsTypeCount;
    switchCountFuncMap_[LayoutEng::TEXTS]            = &ElementsReportDialogViewCtrl::TextsTypeCount;
    switchCountFuncMap_[LayoutEng::DRC_MARKS]        = &ElementsReportDialogViewCtrl::DRCMarksTypeCount;
    switchCountFuncMap_[LayoutEng::DIMENSION]        = &ElementsReportDialogViewCtrl::DimensionTypeCount;
}
void ElementsReportDialogViewCtrl::GroupsTypeCount(
    const std::map< ShowInfos::InfoKeyWords, ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::GROUPS]++;
}
void ElementsReportDialogViewCtrl::ComponentsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::COMPONENTS]++;
}
void ElementsReportDialogViewCtrl::CellsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::CELLS]++;
}
void ElementsReportDialogViewCtrl::ENetsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::ENETS]++;
}
void ElementsReportDialogViewCtrl::NetsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::NETS]++;
}
void ElementsReportDialogViewCtrl::PinsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::PINS]++;
}
void ElementsReportDialogViewCtrl::ViasTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::VIAS]++;
}
void ElementsReportDialogViewCtrl::GuidelinesTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::GUIDELINES]++;
}
void ElementsReportDialogViewCtrl::TracesTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::TRACES]++;
}
void ElementsReportDialogViewCtrl::LinesTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::LINES]++;
}
void ElementsReportDialogViewCtrl::TraceSegsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::TRACE_SEGS]++;
}
void ElementsReportDialogViewCtrl::OtherSegsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::OTHER_SEGS]++;
}
void ElementsReportDialogViewCtrl::AreasTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    if (AreaIsBoundaryLayer(infoData)) {
        return;
    }
    infoListNumber_[LayoutEng::AREAS]++;
}
void ElementsReportDialogViewCtrl::CutoutsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    if (AreaIsBoundaryLayer(infoData)) {
        return;
    }
    infoListNumber_[LayoutEng::CUTOUTS_CAVITIES]++;
}
void ElementsReportDialogViewCtrl::TextsTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::TEXTS]++;
}
void ElementsReportDialogViewCtrl::DRCMarksTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::DRC_MARKS]++;
}
void ElementsReportDialogViewCtrl::DimensionTypeCount(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    Q_UNUSED(infoData)
    infoListNumber_[LayoutEng::DIMENSION]++;
}

void ElementsReportDialogViewCtrl::InitInfoKeyWordsOne()
{
    infoKeyWords_.clear();
    infoKeyWords_[ShowInfos::REFERENCE_DESIGNATOR_INFO]  = "RefDes";
    infoKeyWords_[ShowInfos::CELL_NAME_INFO]             = "Name";
    infoKeyWords_[ShowInfos::COMPONENT_CLASS_INFO]       = "Class";
    infoKeyWords_[ShowInfos::DEVICE_TYPE_INFO]           = "Device Type";
    infoKeyWords_[ShowInfos::VALUE_INFO]                 = "Value";
    infoKeyWords_[ShowInfos::MAX_HIGH_INFO]              = "Max High";
    infoKeyWords_[ShowInfos::CODE_INFO]                  = "Code";
    infoKeyWords_[ShowInfos::LAYER_INFO]                 = "Layer";
    infoKeyWords_[ShowInfos::ZONE_NAME]                  = "Zone name";
    infoKeyWords_[ShowInfos::ZONE_STACKUP_VIEW]          = "Stackup";
    infoKeyWords_[ShowInfos::ZONE_INFO]                  = "Zone";
    infoKeyWords_[ShowInfos::ORIGIN_CENTER_INFO]         = "Origin-xy";
    infoKeyWords_[ShowInfos::PIN_COUNT_INFO]             = "Pin Count";
    infoKeyWords_[ShowInfos::FIXED_INFO]                 = "Fixed";
    infoKeyWords_[ShowInfos::COMPONENT_NAME_INFO]        = "Component";
    infoKeyWords_[ShowInfos::PIN_NAME_INFO]              = "Pin Name";
    infoKeyWords_[ShowInfos::PIN_TYPE_INFO]              = "Pin Type";
    infoKeyWords_[ShowInfos::NET_NAME_INFO]              = "Net";
    infoKeyWords_[ShowInfos::PADSTACK_NAME_INFO]         = "Pads";
    infoKeyWords_[ShowInfos::X_Y_INFO]                   = "(X,Y)";
    infoKeyWords_[ShowInfos::NUMBER_OF_CONNECTIONS_INFO] = "Number of connections";
    infoKeyWords_[ShowInfos::NAME_INFO]                  = "Name";
    infoKeyWords_[ShowInfos::TYPE_INFO]                  = "Type";
    infoKeyWords_[ShowInfos::VOLTAGE_INFO]               = "Voltage";
    infoKeyWords_[ShowInfos::NO_RAT]                     = "No Rat";
    infoKeyWords_[ShowInfos::ELECTRICAL_NET_INFO]        = "Electrical Net";
    infoKeyWords_[ShowInfos::NET_CLASS_INFO]             = "Net Class";
    infoKeyWords_[ShowInfos::NET_GROUP_INFO]             = "Net Group";
    infoKeyWords_[ShowInfos::VIA_COUNT_INFO]             = "Via count";
    infoKeyWords_[ShowInfos::AREA_FILS_COUNT_INFO]       = "Area Count";
    infoKeyWords_[ShowInfos::FREQUENCY_INFO]             = "Frequency";
    infoKeyWords_[ShowInfos::NETLIST_INFO]               = "Netlist";
    infoKeyWords_[ShowInfos::END_POINT_INFO]             = "End Point";
    InitInfoKeyWordsTwo();
}

void ElementsReportDialogViewCtrl::InitInfoKeyWordsTwo()
{
    infoKeyWords_[ShowInfos::START_POINT_INFO]        = "Start Point";
    infoKeyWords_[ShowInfos::END_ORIGIN_INFO]         = "End origin";
    infoKeyWords_[ShowInfos::LENGTH_INFO]             = "Length";
    infoKeyWords_[ShowInfos::ID_INFO]                 = "ID";
    infoKeyWords_[ShowInfos::WIDTH_INFO]              = "Width";
    infoKeyWords_[ShowInfos::RADIUS_INFO]             = "Radius";
    infoKeyWords_[ShowInfos::AREA_SIZE_INFO]          = "Area Size";
    infoKeyWords_[ShowInfos::CONNECTED_LINES_INFO]    = "Connected lines";
    infoKeyWords_[ShowInfos::CONNECTED_VIAS_INFO]     = "Connected vias";
    infoKeyWords_[ShowInfos::CONNECTED_PINS_INFO]     = "Connected pins";
    infoKeyWords_[ShowInfos::CONNECTED_AREAS_INFO]    = "Connected areas";
    infoKeyWords_[ShowInfos::DIMENSION_INFO]          = "Dimensions";
    infoKeyWords_[ShowInfos::ANCHOR_POINT_INFO]       = "Anchor points";
    infoKeyWords_[ShowInfos::REF_DATUM_INFO]          = "Reference datum";
    infoKeyWords_[ShowInfos::FILLED_TYPE_INFO]        = "Filled Type";
    infoKeyWords_[ShowInfos::TEARDROP_TYPE_INFO]      = "Teardrop Type";
    infoKeyWords_[ShowInfos::NUMBER_OF_VOIDS_INFO]    = "Number of voids";
    infoKeyWords_[ShowInfos::NUMBER_OF_SEGMENTS_INFO] = "Number of Segments";
    infoKeyWords_[ShowInfos::TEXT_CONTEXTS_INFO]      = "Contexts";
    infoKeyWords_[ShowInfos::JUSTIFICATION_INFO]      = "Justification";
    infoKeyWords_[ShowInfos::TEXT_BLOCK_INFO]         = "Text_block#";
    infoKeyWords_[ShowInfos::CONSTRAINT_INFO]         = "Constraint";
    infoKeyWords_[ShowInfos::CONSTRAINT_SET_INFO]     = "Constraint set";
    infoKeyWords_[ShowInfos::CONSTRAINT_TYPE_INFO]    = "Constraint Type";
    infoKeyWords_[ShowInfos::CONSTRAINT_VALUE_INFO]   = "Constraint value";
    infoKeyWords_[ShowInfos::ACTUAL_VALUE_INFO]       = "Actual value";
    infoKeyWords_[ShowInfos::ELEMENT_TYPE_INFO]       = "Element type";
    infoKeyWords_[ShowInfos::ROTATION_INFO]           = "Rotation";
    infoKeyWords_[ShowInfos::TESTPOINT_NUM_INFO]      = "Testpoints number";
    infoKeyWords_[ShowInfos::TESTPOINT_LAYER_INFO]    = "Testpoint layer";
    infoKeyWords_[ShowInfos::LOCK_STATUS]             = "Lock status";
    infoKeyWords_[ShowInfos::PROPERTY_INFO]           = "Property Info";
    infoKeyWords_[ShowInfos::PLACEMENT_STATUS]        = "Placement Status";
    infoKeyWords_[ShowInfos::PIN_INFO]                = "Pin Info";
    infoKeyWords_[ShowInfos::VIAS_INFO]               = "Vias";
    infoKeyWords_[ShowInfos::TRACES_INFO]             = "Traces";
    infoKeyWords_[ShowInfos::AREAS_INFO]              = "Areas";
    infoKeyWords_[ShowInfos::AREAS_PRIORITY]          = "Priority id";
    infoKeyWords_[ShowInfos::REGION_INFO]             = "Region List";
    infoKeyWords_[ShowInfos::SEG_COUNT]               = "Segment count";
    InitInfoKeyWordsThr();
}

void ElementsReportDialogViewCtrl::InitInfoKeyWordsThr()
{
    infoKeyWords_[ShowInfos::SCHEDULE_INFO]               = "Scheduled";
    infoKeyWords_[ShowInfos::AREA_TYPE_INFO]              = "Area type";
    infoKeyWords_[ShowInfos::IS_DISABLED_INFO]            = "Is Disabled";
    infoKeyWords_[ShowInfos::CONNECTIONS]                 = "Connections";
    infoKeyWords_[ShowInfos::NET_PATH_LENGTH_INFO]        = "Net path length";
    infoKeyWords_[ShowInfos::TOTAL_ETCH_LENGTH]           = "Total etch length";
    infoKeyWords_[ShowInfos::TOTAL_PATH_LENGTH]           = "Total path length";
    infoKeyWords_[ShowInfos::TOTAL_MANHATTAN_LENGTH_INFO] = "Total manhattan length";
    infoKeyWords_[ShowInfos::PERCENT_MANHATTAN_INFO]      = "Percent manhattan";
    infoKeyWords_[ShowInfos::GROUP_NAME_INFO]             = "Group Name";
    infoKeyWords_[ShowInfos::NUMBER_OF_ETCH_SHAPES_INFO]  = "Number of etch shapes";
    infoKeyWords_[ShowInfos::BOUNDARY_SEG_INFO]           = "Area boundary segment";
    infoKeyWords_[ShowInfos::TESTPOINT_INFO]              = "Testpoints list";
    infoKeyWords_[ShowInfos::PROBE_TYPE]                  = "Probe type";
    infoKeyWords_[ShowInfos::TESTPOINT_IS_ON_TRACE]       = "Testpoint";
    infoKeyWords_[ShowInfos::ORIGIN_XY_INFO]              = "Origin-xy";
    infoKeyWords_[ShowInfos::IS_MIRROR]                   = "IsMirror";
    infoKeyWords_[ShowInfos::MEMBEROFENET]                = "Member of ENet";
    infoKeyWords_[ShowInfos::HATCH_STYLE]                 = "Hatch Style";
    infoKeyWords_[ShowInfos::HATCH_SET_1]                 = "Hatch Set #1";
    infoKeyWords_[ShowInfos::HATCH_SET_2]                 = "Hatch Set #2";
    infoKeyWords_[ShowInfos::HATCH_FIR_WIDTH]             = "Hatch Set #1: wtidth";
    infoKeyWords_[ShowInfos::HATCH_FIR_SPACING]           = "Hatch Set #1: spacing";
    infoKeyWords_[ShowInfos::HATCH_FIR_ANGLE]             = "Hatch Set #1: angle";
    infoKeyWords_[ShowInfos::HATCH_SEC_WIDTH]             = "Hatch Set #2: wtidth";
    infoKeyWords_[ShowInfos::HATCH_SEC_SPACING]           = "Hatch Set #2: spacing";
    infoKeyWords_[ShowInfos::HATCH_SEC_ANGLE]             = "Hatch Set #2: angle";
    infoKeyWords_[ShowInfos::HATCH_ORIGIN_X_Y]            = "Hatch origin";
    infoKeyWords_[ShowInfos::NORMALIZE_ANGLE]             = "Normalize angle";
    infoKeyWords_[ShowInfos::IS_WAIVED_DRC]               = "Is Waived";
    infoKeyWords_[ShowInfos::WAIVE_DRC_COMMENT]           = "Comment";
    infoKeyWords_[ShowInfos::COMPONENT_PROPERTY]          = "Component Property";
    infoKeyWords_[ShowInfos::CELL_PROPERTY]               = "Cell Property";
    infoKeyWords_[ShowInfos::ELECTRICAL_CONS_ASSIGN]      = "";  //该键不显示
    infoKeyWords_[ShowInfos::CONSTRAINT_INFOMATION]       = "Constraint information";
    infoKeyWords_[ShowInfos::MEMBER_OF_GROUPS]            = "Member of group";
    infoKeyWords_[ShowInfos::GUIDLINE_POINT_INFO]         = "";  // 该键不显示
    infoKeyWords_[ShowInfos::CLINE_POINT_INFO]            = "";  // 该键不显示
    infoKeyWords_[ShowInfos::NCLINE_POINT_INFO]           = "";  // 该键不显示
    infoKeyWords_[ShowInfos::PIN_INFO_LIST]               = "Pin List";
    infoKeyWords_[ShowInfos::VIA_STACK_INFO]              = "Part of a via stack";
    infoKeyWords_[ShowInfos::SEGMENTS_COUNT_INFO]         = "Segments";
    infoKeyWords_[ShowInfos::USER_NAME]                   = "Lock User";
    infoKeyWords_[ShowInfos::ARC_CENTER_INFO]             = "Center";
    infoKeyWords_[ShowInfos::PIN_DRILL_TYPE]              = "Drill type";
    infoKeyWords_[ShowInfos::VIA_DRILL_TYPE]              = "Drill type";
}

QString ElementsReportDialogViewCtrl::TransTextAndEnd(QString const& data) { return data + "<br>"; }

QString ElementsReportDialogViewCtrl::TransNextLineAndEnd(QString const& data)
{
    return "<tr><td></td>" + EIGHT_SPACE + data + "<td></td></tr>";
}

QString ElementsReportDialogViewCtrl::TransStartText(QString const& data) { return EIGHT_SPACE + EIGHT_SPACE + data; }

QString ElementsReportDialogViewCtrl::TransNoKeyStartText(QString const& data) { return EIGHT_SPACE + data; }

void ElementsReportDialogViewCtrl::AddRowData(QString& text, const QString& firstPar, const QString& secondPar)
{
    text += "<div><td >" + firstPar + "</td><td>" + secondPar + "</td></div>";
}
void ElementsReportDialogViewCtrl::AddSpaceRowData(QString& text, const QString& firstPar, const QString& secondPar)
{
    MakeHtmlText(text, "<div><td width=230>" + EIGHT_SPACE + firstPar + "</td><td>" + secondPar + "</td></div>");
}

QString ElementsReportDialogViewCtrl::TransHyperLink(QString const& head, QString const& link, QString const& doId)
{
    return "<a href='" + head + link + "doId:" + doId + "'style='color:#368ccb;'>" + link + "</a>";
}

QString ElementsReportDialogViewCtrl::TransLine() { return "<hr></hr>"; }

QString ElementsReportDialogViewCtrl::TransPinListStart(QString const& data)
{
    return "<div><td width=230>" + EIGHT_SPACE + EIGHT_SPACE + data + "</td>";
}

QString ElementsReportDialogViewCtrl::TransPinListEnd(QString const& data) { return "<td>" + data + "</td></div>"; }

void ElementsReportDialogViewCtrl::InitValueFuncMap()
{
    switchValueFuncMap_.clear();
    switchValueFuncMap_[LayoutEng::GROUPS]           = &ElementsReportDialogViewCtrl::NormalTypeValue;
    switchValueFuncMap_[LayoutEng::COMPONENTS]       = &ElementsReportDialogViewCtrl::NormalTypeValue;
    switchValueFuncMap_[LayoutEng::CELLS]            = &ElementsReportDialogViewCtrl::NormalPointTypeValue;
    switchValueFuncMap_[LayoutEng::ENETS]            = &ElementsReportDialogViewCtrl::NormalTypeValue;
    switchValueFuncMap_[LayoutEng::NETS]             = &ElementsReportDialogViewCtrl::NetsTypeValue;
    switchValueFuncMap_[LayoutEng::PINS]             = &ElementsReportDialogViewCtrl::NormalPointTypeValue;
    switchValueFuncMap_[LayoutEng::VIAS]             = &ElementsReportDialogViewCtrl::NormalPointTypeValue;
    switchValueFuncMap_[LayoutEng::GUIDELINES]       = &ElementsReportDialogViewCtrl::GuidelinesTypeValue;
    switchValueFuncMap_[LayoutEng::TRACES]           = &ElementsReportDialogViewCtrl::TracesOrLinesTypeValue;
    switchValueFuncMap_[LayoutEng::LINES]            = &ElementsReportDialogViewCtrl::TracesOrLinesTypeValue;
    switchValueFuncMap_[LayoutEng::TRACE_SEGS]       = &ElementsReportDialogViewCtrl::OtherOrTraceSegTypeValue;
    switchValueFuncMap_[LayoutEng::OTHER_SEGS]       = &ElementsReportDialogViewCtrl::OtherOrTraceSegTypeValue;
    switchValueFuncMap_[LayoutEng::AREAS]            = &ElementsReportDialogViewCtrl::AreasOrCutoutTypeValue;
    switchValueFuncMap_[LayoutEng::CUTOUTS_CAVITIES] = &ElementsReportDialogViewCtrl::AreasOrCutoutTypeValue;
    switchValueFuncMap_[LayoutEng::TEXTS]            = &ElementsReportDialogViewCtrl::NormalPointTypeValue;
    switchValueFuncMap_[LayoutEng::DRC_MARKS]        = &ElementsReportDialogViewCtrl::NormalPointTypeValue;
    switchValueFuncMap_[LayoutEng::DIMENSION]        = &ElementsReportDialogViewCtrl::NormalPointTypeValue;
}

void ElementsReportDialogViewCtrl::NormalTypeValue(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text)
{
    for (auto value : values) {
        if (!value.second.info.empty()) {
            if (value.first == LayoutEng::ShowInfos::ORIGIN_CENTER_INFO) {
                DoWithNormalPointInfo(value.first, QString::fromStdString(value.second.info[0].data),
                                      value.second.info[0].id, text);
            } else {
                DoWithNormalsInfo(value.first, QString::fromStdString(value.second.info[0].data), text);
            }
        }
    }
    MakeHtmlText(text, TransLine());
}

void ElementsReportDialogViewCtrl::NormalPointTypeValue(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text)
{
    for (auto value : values) {
        if (!value.second.info.empty()) {
            if (value.first == LayoutEng::ShowInfos::ORIGIN_CENTER_INFO ||
                value.first == LayoutEng::ShowInfos::ORIGIN_XY_INFO || value.first == LayoutEng::ShowInfos::X_Y_INFO) {
                DoWithNormalPointInfo(value.first, QString::fromStdString(value.second.info[0].data),
                                      value.second.info[0].id, text);
            } else if ((value.first == LayoutEng::ShowInfos::ANCHOR_POINT_INFO) ||
                       (value.first == LayoutEng::ShowInfos::REF_DATUM_INFO)) {
                DoWithIncludePointInfo(value.first, value.second.info, text);
            } else {
                DoWithNormalsInfo(value.first, QString::fromStdString(value.second.info[0].data), text);
            }
        }
    }
    MakeHtmlText(text, TransLine());
}

void ElementsReportDialogViewCtrl::NetsTypeValue(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text)
{
    for (auto value : values) {
        if (!value.second.info.empty()) {
            if (value.first == LayoutEng::ShowInfos::TESTPOINT_INFO ||
                value.first == LayoutEng::ShowInfos::CONSTRAINT_INFOMATION) {
                DoWithIncludePointInfo(value.first, value.second.info, text);
            } else if (value.first == LayoutEng::ShowInfos::ELECTRICAL_CONS_ASSIGN) {
                DoWithElectricalInfo(QString::fromStdString(value.second.info[0].data), text);
            } else if (value.first == LayoutEng::ShowInfos::PIN_INFO_LIST) {
                DoWithPinListValue(value.first, value.second.info, text);
            } else {
                DoWithNormalsInfo(value.first, QString::fromStdString(value.second.info[0].data), text);
            }
        }
    }
    MakeHtmlText(text, TransLine());
}

void ElementsReportDialogViewCtrl::GuidelinesTypeValue(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text)
{
    for (auto value : values) {
        if (!value.second.info.empty()) {
            if (value.first == LayoutEng::ShowInfos::GUIDLINE_POINT_INFO) {
                DoWithIncludePointInfo(value.first, value.second.info, text);
            } else if (value.first == LayoutEng::ShowInfos::START_POINT_INFO ||
                       value.first == LayoutEng::ShowInfos::END_POINT_INFO) {
                continue;
            } else {
                DoWithNormalsInfo(value.first, QString::fromStdString(value.second.info[0].data), text);
            }
        }
    }
    MakeHtmlText(text, TransLine());
}

void ElementsReportDialogViewCtrl::TracesOrLinesTypeValue(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text)
{
    for (auto value : values) {
        if (!value.second.info.empty()) {
            if (value.first == LayoutEng::ShowInfos::CONNECTIONS ||
                value.first == LayoutEng::ShowInfos::CLINE_POINT_INFO ||
                value.first == LayoutEng::ShowInfos::NCLINE_POINT_INFO) {
                DoWithIncludePointInfo(value.first, value.second.info, text);
            } else if (value.first == LayoutEng::ShowInfos::WIDTH_INFO) {
                continue;
            } else {
                DoWithNormalsInfo(value.first, QString::fromStdString(value.second.info[0].data), text);
            }
        }
    }
    MakeHtmlText(text, TransLine());
}

void ElementsReportDialogViewCtrl::OtherOrTraceSegTypeValue(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text)
{
    for (auto value : values) {
        if (!value.second.info.empty()) {
            if (value.first == LayoutEng::ShowInfos::START_POINT_INFO ||
                value.first == LayoutEng::ShowInfos::END_POINT_INFO ||
                value.first == LayoutEng::ShowInfos::ARC_CENTER_INFO) {
                DoWithNormalPointInfo(value.first, QString::fromStdString(value.second.info[0].data),
                                      value.second.info[0].id, text);
            } else if (value.first == LayoutEng::ShowInfos::BOUNDARY_SEG_INFO) {
                DoWithIncludePointInfo(value.first, value.second.info, text);
            } else if (value.first == LayoutEng::ShowInfos::SEGMENTS_COUNT_INFO) {
                continue;
            } else {
                DoWithNormalsInfo(value.first, QString::fromStdString(value.second.info[0].data), text);
            }
        }
    }
    MakeHtmlText(text, TransLine());
}

void ElementsReportDialogViewCtrl::AreasOrCutoutTypeValue(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values, QString& text)
{
    if (AreaIsBoundaryLayer(values)) {
        return;
    }
    for (auto& value : values) {
        if (!value.second.info.empty()) {
            if (value.first == LayoutEng::ShowInfos::BOUNDARY_SEG_INFO) {
                DoWithIncludePointInfo(value.first, value.second.info, text);
            } else {
                DoWithNormalsInfo(value.first, QString::fromStdString(value.second.info[0].data), text);
            }
        }
    }
    MakeHtmlText(text, TransLine());
}

void ElementsReportDialogViewCtrl::InitTextValue(uint32_t index, std::map< FindType, QString >& textValue)
{
    InitListing(index, LayoutEng::GROUPS, "groups", textValue);
    InitListing(index, LayoutEng::COMPONENTS, "components", textValue);
    InitListing(index, LayoutEng::CELLS, "cells", textValue);
    InitListing(index, LayoutEng::ENETS, "enets", textValue);
    InitListing(index, LayoutEng::NETS, "nets", textValue);
    InitListing(index, LayoutEng::PINS, "pins", textValue);
    InitListing(index, LayoutEng::VIAS, "vias", textValue);
    InitListing(index, LayoutEng::GUIDELINES, "guidelines", textValue);
    InitListing(index, LayoutEng::TRACES, "traces", textValue);
    InitListing(index, LayoutEng::LINES, "lines", textValue);
    InitListing(index, LayoutEng::TRACE_SEGS, "trace segs", textValue);
    InitListing(index, LayoutEng::OTHER_SEGS, "other segs", textValue);
    InitListing(index, LayoutEng::AREAS, "areas", textValue);
    InitListing(index, LayoutEng::CUTOUTS_CAVITIES, "cutouts/cavities", textValue);
    InitListing(index, LayoutEng::TEXTS, "texts", textValue);
    InitListing(index, LayoutEng::DRC_MARKS, "DRC marks", textValue);
    InitListing(index, LayoutEng::DIMENSION, "dimension", textValue);
    InitListing(index, LayoutEng::BLOCK_AREA, "block area", textValue);
    InitListing(index, LayoutEng::TESTPOINT, "testPoint", textValue);
    InitListing(index, LayoutEng::TPOINTS, "tPoints", textValue);
    InitListing(index, LayoutEng::OBSTACLES, "obstacles", textValue);
}

void ElementsReportDialogViewCtrl::InitListing(uint32_t index,
                                               const LayoutEng::FindType& enumType,
                                               const QString& stringType,
                                               std::map< FindType, QString >& textValue)
{
    for (auto& it : dialogInfoListNumber_) {
        if (it.first == index) {
            if (it.second[enumType] > 0) {
                QString listingStr  = "Listing : ";
                QString numberStr   = QString::number(it.second[enumType]) + " elements " + stringType;
                textValue[enumType] = "<div><td width=230>" + listingStr + "</td>" + "<td>" + numberStr + "</td></div>";
            }
            break;
        }
    }
}

void ElementsReportDialogViewCtrl::InitialCliFnCf()
{
    funcMap_[LayoutGui::ELEMENTS_REPORT_CFG_CLOSE] = &ElementsReportDialogViewCtrl::CfgCloseCmd;
    funcMap_[CMD_NAME_INFO_FIXED_STATUS]           = &ElementsReportDialogViewCtrl::CmdChangeDlgFixedStatus;
}

void ElementsReportDialogViewCtrl::CfgCloseCmd(const std::string& value)
{
    Q_UNUSED(value);
    if (dialogList_.empty()) {
        return;
    }
    if (GetLastDialog(dialogList_) == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Last dialog is nullptr.")
        return;
    }
    GetLastDialog(dialogList_)->close();
}

void ElementsReportDialogViewCtrl::CmdChangeDlgFixedStatus(const std::string& value)
{
    bool isDialogFixed = false;
    if (value == "fixed") {
        isDialogFixed = true;
    } else if (value == "unfixed") {
        isDialogFixed = false;
    } else {
        PWARNING(ErrorCode::RET_INVALID, "The recorded text is incorrect.")
    }
    if (GetLastDialog(dialogList_) == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Last dialog is nullptr!")
        return;
    }
    GetLastDialog(dialogList_)->SetDialogFixedStatus(isDialogFixed);
}
