
#include "elements_report_dialog_view_ctrl.h"
#include <QApplication>
#include "elements_report_dialog.h"
#include "gui_common.h"
#include "select_find_api.h"
#include "setting_api.h"
#include "widget_manager.h"
#include "view_ctrl_common.h"
#include "layout_common.h"
#include "placement_api.h"
#include "info_api.h"
#include "view_ctrl_emitter.h"
#include "cmd_interface.h"
#include "layout_engine.h"
#include "edit_api.h"
#include "cmd_notify_interface.h"
#include "constraint_api.h"
#include "cmd_keywords.h"
#include "cli_notify_interface.h"
#include "zoom_in_interface.h"
#include "c_line.h"
#include "via.h"
#include "pin.h"
#include "c_area.h"
#include "hatch_line.h"
#include "common_sw.h"
#include "layer_interface.h"
#include "connect_relation.h"
#include "public_tool.h"
#include "project_interface.h"
#include "view_window_tab_view_ctrl.h"

using namespace std;
using namespace LayoutEng;
using namespace DesignDatabase;
using namespace ViewCtrlCommon;
using namespace LayoutCmdEngine;
using namespace LayoutCliEngine;
using namespace LayoutGui;
using namespace LayoutViewCtrl;

namespace {
const int VIOLATION_COUNT            = 2;
const int MAX_DISPLAYED_CHARACTERS   = 1000000;
const int INFO_OPEN_MAXIMUM_NUMBER   = 19;
const QString SPACE                  = "&nbsp;";
const QString NEXTLINE               = "<br>";
const QString INFO_OPEN_MAXIMUM      = "A maximum of twenty info windows can be opened.";
const std::string DRC_LAYER_NAME_ALL = "All";
const std::string DATA_OBJ_TYPE_NET  = "NET";
std::map< FindType, QString > ITEM_TYPE_MAP {{FindType::GROUPS, "Groups"},
                                             {FindType::COMPONENTS, "Components"},
                                             {FindType::CELLS, "Cells"},
                                             {FindType::PINS, "Pins"},
                                             {FindType::VIAS, "Vias"},
                                             {FindType::NETS, "Nets"},
                                             {FindType::ENETS, "Enets"},
                                             {FindType::GUIDELINES, "Guidelines"},
                                             {FindType::TRACES, "Traces"},
                                             {FindType::LINES, "Lines"},
                                             {FindType::TRACE_SEGS, "Trace segs"},
                                             {FindType::OTHER_SEGS, "Other segs"},
                                             {FindType::AREAS, "Areas"},
                                             {FindType::CUTOUTS_CAVITIES, "Cutouts/Cavities"},
                                             {FindType::TEXTS, "Texts"},
                                             {FindType::DRC_MARKS, "DRC Marks"},
                                             {FindType::DIMENSION, "Dimension"}};
std::vector< DataDict::DataObjType > CENTER_POINT_TYPE {
    DataDict::DataObjType::DATA_CLINE_SEG, DataDict::DataObjType::DATA_NCSEG, DataDict::DataObjType::DATA_COPPERAREA,
    DataDict::DataObjType::DATA_TPOINT, DataDict::DataObjType::DATA_GUIDELINE};
}  // namespace

ElementsReportDialogViewCtrl::ElementsReportDialogViewCtrl()
    : ViewCtrl(ViewCtrlCommon::ELEMENTS_REPORTS_DIALOG_VIEW_CTRL)
{}

void ElementsReportDialogViewCtrl::GetDefaultFilePath(QString& filePath)
{
    std::string path;
    if (LayoutEng::GetExportDir(path) != ErrorCode::RET_OK) {
        PWARNING(ErrorCode::RET_FAILURE, "Failed to get default path");
        return;
    }
    filePath = QString::fromStdString(path);
}

void ElementsReportDialogViewCtrl::ShowInfoDialog(
    const vector< map< FindType, map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoValues)
{
    Initial(*WidgetManager::GetInst().GetWidget(MAIN_WINDOW_VIEW_CTRL), infoValues);
}

void ElementsReportDialogViewCtrl::CloseDialog()
{
    // endCmd时，若最后一个窗口被固定则不关闭，若不固定则关且只关最后一个窗口。
    if (GetLastDialog(dialogList_) != nullptr && !GetLastDialog(dialogList_)->isHidden()) {
        if (!GetLastDialog(dialogList_)->GetDialogFixedStatus()) {
            GetLastDialog(dialogList_)->close();
        }
    }
}

ElementsReportDialogViewCtrl& ElementsReportDialogViewCtrl::GetInst()
{
    static ElementsReportDialogViewCtrl inst;
    return inst;
}

ElementsReportDialogViewCtrl::~ElementsReportDialogViewCtrl() {}

void ElementsReportDialogViewCtrl::MakeTopWidget() {}

void ElementsReportDialogViewCtrl::Initial(
    QWidget& parent,
    const vector< map< FindType, map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoValues)
{
    // add by anling
    allInfo_ = infoValues;
    InitTypeCountFuncMap();
    InitValueFuncMap();
    InitInfoKeyWordsOne();
    if (GetLastDialog(dialogList_) == nullptr) {
        ElementsReportDialog* dialog = new ElementsReportDialog(&parent);
        NewElementsReportDialog(dialog);
        dialog->show();  // 打开版图，第一次弹出info窗口的位置为中间；
        popUpDlgPos_ = dialog->pos();
        ClickFnCf();
        return;
    }
    if (dialogList_.size() > INFO_OPEN_MAXIMUM_NUMBER &&
        GetLastDialog(dialogList_)->GetDialogFixedStatus()) {  // info窗口最多打开20个
        ShowWarning(INFO_OPEN_MAXIMUM);
        return;
    }
    if (GetLastDialog(dialogList_)->GetNeedCreateDialogStatus()) {  // 最后一个窗口关闭，则重新new
        ElementsReportDialog* dialog = new ElementsReportDialog(&parent);
        NewElementsReportDialog(dialog);
        MoveNewDialogPos(dialog);
        ClickFnCf();
    } else {
        // 如果最后一个info窗口是固定的则重新new出来一个新的info窗口
        if (GetLastDialog(dialogList_)->GetDialogFixedStatus()) {
            ElementsReportDialog* dialog = new ElementsReportDialog(&parent);
            NewElementsReportDialog(dialog);
            MoveNewDialogPos(dialog);
            ClickFnCf();
            return;
        }
        ClearDisplayCount(GetLastDialog(dialogList_)->GetDialogIndex());
        IntialSliceData(GetLastDialog(dialogList_)->GetDialogIndex());
        GetTypeListCount(GetLastDialog(dialogList_)->GetDialogIndex(), allInfo_);
        UpdateDialogAllInfo();
        InitToSetData(GetLastDialog(dialogList_));
        MoveLastDialogPos();
        ClickFnCf();
    }
}

void ElementsReportDialogViewCtrl::InitialConnect(ElementsReportDialog* dialog)
{
    connect(dialog, &ElementsReportDialog::GetDefaultFilePathSg, this,
            &ElementsReportDialogViewCtrl::GetDefaultFilePath);
    connect(dialog, &ElementsReportDialog::CloseElementsReportDialogSg, this,
            &ElementsReportDialogViewCtrl::CloseElementsReportDialog);
    connect(dialog, &ElementsReportDialog::SendDoIdSg, this, &ElementsReportDialogViewCtrl::HighLight);
    connect(dialog, &ElementsReportDialog::TypeComboxItemChangeSg, this,
            &ElementsReportDialogViewCtrl::ElementTypeChange);
    connect(dialog, &ElementsReportDialog::TextBrowserPageRightSg, this,
            &ElementsReportDialogViewCtrl::UpdateNextPageData, Qt::UniqueConnection);
    connect(dialog, &ElementsReportDialog::TextBrowserPageLeftSg, this,
            &ElementsReportDialogViewCtrl::UpdatePreviousPageData, Qt::UniqueConnection);
    connect(dialog, &ElementsReportDialog::SaveFileSg, this, &ElementsReportDialogViewCtrl::SaveInfoFile);
    connect(dialog, &ElementsReportDialog::RecordDlgFixedStatusSg, this,
            &ElementsReportDialogViewCtrl::RecordDlgFixedStatus);
    connect(&ProjectInterface::GetInst(), &ProjectInterface::CloseDdbSg, this,
            &ElementsReportDialogViewCtrl::CloseAllDialog, Qt::UniqueConnection);
    connect(&ViewCtrlEmitter::GetInstance(), &ViewCtrlEmitter::ServerHideMainWindowCloseDialogSg, this,
            &ElementsReportDialogViewCtrl::CloseAllDialog, Qt::UniqueConnection);
}

bool ElementsReportDialogViewCtrl::ItemRelatedNet(const FindType& type)
{
    if (type == FindType::PINS || type == FindType::VIAS || type == FindType::TRACES || type == FindType::AREAS ||
        type == FindType::GUIDELINES) {
        return true;
    } else {
        return false;
    }
}

void ElementsReportDialogViewCtrl::ShowNetsInfo(const std::vector< std::string >& netNames,
                                                bool status,
                                                bool isDoubleClickPPR,
                                                std::vector< DesignDatabase::DataObjectId > pinpairIds)
{
    if (netNames.empty()) {
        return;
    }
    // 1.0 hightlight the elements of the net
    SelectNets(netNames, status, isDoubleClickPPR, pinpairIds);
    // 2.0 show the infomation of the net
    GetAndShowNetsInfo(netNames);
}

void ElementsReportDialogViewCtrl::SelectNets(const std::vector< std::string >& netNames,
                                              bool status,
                                              bool isDoubleClickPPR,
                                              std::vector< DesignDatabase::DataObjectId > pinpairIds)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));  // by add tianhe 2022/01/02
    string netName;
    Rect showRect;
    showRect.ResetByCoord(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
    CmdId cmdId = LayoutCmdEngine::CmdInterface::GetInstance().GetCurrentCmd();
    LayoutEng::ClearHighlightedElements();  // first un-hightlight all element
    if (isDoubleClickPPR) {
        for (auto iter : pinpairIds) {
            HighlightPinpair(iter);
        }
    } else {
        HighlightNet(netNames, status);
    }

    if (cmdId == CmdId::CMD_INFO) {
        GetAndShowNetsInfo(netNames);
    } else if (cmdId == CmdId::CMD_LOCK) {
        for (uint32_t i = 0; i < netNames.size(); ++i) {
            LockElement(netNames[i]);
        }
    } else if (cmdId == CmdId::CMD_UNLOCK) {
        for (uint32_t i = 0; i < netNames.size(); ++i) {
            UnlockElement(netName);
        }
    }
    if (!isDoubleClickPPR) {
        ProcessHilightCmd(netNames);
    }

    for (uint32_t i = 0; i < netNames.size(); ++i) {
        netName = netNames.at(i);
        if (netName.size() == 0) {
            continue;
        }
        ExpandNetRectByRect(netName, showRect);
    }
    if (showRect.leftBottom_.GetX() == INT_MAX) {  // 修改select选择的net在绘图区高亮失败的判断方法
        QApplication::restoreOverrideCursor();
        return;
    }
    QRectF rectF = ShowRectInCentre(showRect);
    emit ViewCtrlEmitter::GetInstance().CentreShowInViewWinSg(rectF.normalized());
    QApplication::restoreOverrideCursor();  // by add tianhe 2022/01/03
}

uint32_t ElementsReportDialogViewCtrl::GetViewMoveLocation(DataObjectId drcId, QPointF& point)
{
    LayoutEng::DrcViolation* drc = GET_DO_RO(LayoutEng::DrcViolation, drcId);
    if (drc == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Failed to get drc violation ptr: ", drcId)
        return ErrorCode::RET_NULL_PTR;
    }
    Point drcLoc = drc->GetLocation(drc);
    double scale = StorageAccScaleToDou();
    point.setX(static_cast< double >(drcLoc.GetX()) / scale);
    point.setY(static_cast< double >(drcLoc.GetY()) / scale);
    return ErrorCode::RET_OK;
}

void ElementsReportDialogViewCtrl::SelecrtDrcVio(DataObjectId drcId)
{
    DrcCloseAndOpenLayers(drcId);  // 处理关闭打开层
    Rect showRect;
    showRect.ResetByCoord(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
    LayoutEng::ClearHighlightedElements();
    HighlightDrcVio(drcId);

    // add by fengjiabao 2024-06-13
    QPointF drcLoc;
    uint32_t ret = GetViewMoveLocation(drcId, drcLoc);
    if (ret != ErrorCode::RET_OK) {
        PERROR(ret, "Failed to get drc Violation location.    ", drcId)
        return;
    }
    ViewWindowTabViewCtrl::GetInst().SetCenterPos(drcLoc);

    // add by anling 2023/01/10
    HighLightConsNetName(drcId);
}

void ElementsReportDialogViewCtrl::DrcCloseAndOpenLayers(DesignDatabase::DataObjectId drcId)
{
    std::vector< DesignDatabase::DataObjectId > allVisibleLayers = LayerInterface::GetInst().GetAllVisibleLayers();
    //获取drc所在层
    std::string drcLayerName;
    Constraint::GetPhyLayerName(drcId, drcLayerName);
    std::set< uint64_t > openLayerIds {};
    openLayerIds.insert(LayerInterface::GetInst().GetLayerId(LayerKeyWords::LAYER_DRC + "/" + drcLayerName));
    //获取obj所在层
    std::vector< uint64_t > layersVec {};
    GetElementLayers(drcId, allVisibleLayers, layersVec);

    //打开所有层
    layersVec.insert(layersVec.end(), openLayerIds.begin(), openLayerIds.end());
    LayerInterface::GetInst().SetLayerVisible(layersVec, true);
}

void ElementsReportDialogViewCtrl::GetElementLayers(DataObjectId drcId,
                                                    const std::vector< DataObjectId >& allVisibleLayers,
                                                    std::vector< uint64_t >& layersVec)
{
    std::set< uint64_t > openLayerIds;
    std::vector< DesignDatabase::DataObjectId > violationIds;
    uint32_t vioRet = Constraint::GetViolations(drcId, violationIds);
    if (vioRet != ErrorCode::RET_OK) {
        PERROR(vioRet, "Failed to get obj id by drcId.")
        return;
    }
    if (violationIds.size() > VIOLATION_COUNT) {
        PERROR(ErrorCode::RET_INVALID, "The violation count is error.")
        return;
    }
    DesignDatabase::DataObjectId netId = 0;
    GetInfoByViolationIds(violationIds, netId);
    std::string drcLayerName;
    Constraint::GetPhyLayerName(drcId, drcLayerName);
    if (netId) {  // 两个对象中有一个是net，则打开net所连的铜皮和走线所在的层
        GetLayerIdByNetId(netId, openLayerIds);
    } else {
        if (drcLayerName == "All") {
            GetAllDrcPinViaLayerId(violationIds, allVisibleLayers, openLayerIds);
        } else {
            GetNotAllDrcElementLayer(drcLayerName, violationIds, openLayerIds);
        }
    }
    layersVec.clear();
    layersVec.assign(openLayerIds.begin(), openLayerIds.end());
}

void ElementsReportDialogViewCtrl::GetInfoByViolationIds(std::vector< DesignDatabase::DataObjectId >& violationIds,
                                                         DesignDatabase::DataObjectId& netId)
{
    for (const auto& objId : violationIds) {
        DataDict::DataObjType elementType = ElementBase::GetEleType(objId);
        if (elementType != DataDict::DATA_CONSTAINT_GROUP) {
            if (elementType == DataDict::DATA_NET) {
                netId = objId;
            }
            continue;
        }
        std::string pbjName;
        std::string objType;
        uint32_t objRet = Constraint::GetObjNameAndTypeById(objId, pbjName, objType);
        if (objRet != ErrorCode::SUCCESS) {
            PERROR(objRet, "Failed to get the consgroup data.");
            continue;
        }
        if (objType == DATA_OBJ_TYPE_NET) {
            uint32_t netRet = Constraint::GetNetIdByConsGroupId(objId, netId);
            if (netRet != ErrorCode::SUCCESS) {
                PERROR(netRet, "Failed to get the net id by consgroup id.");
                continue;
            }
            break;
        }
    }
}

void ElementsReportDialogViewCtrl::GetLayerIdByNetId(DataObjectId netId, std::set< DataObjectId >& openLayerIds)
{
    Net* nets = GET_DO_RO(Net, netId);
    if (nets == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "net is nullptr, net = ", netId)
        return;
    }
    std::vector< DataObjectId > clines {};
    std::vector< DataObjectId > careas {};
    nets->GetClines(clines);
    nets->GetCAreas(careas);
    std::set< DataObjectId > elementLayerId {};
    GetLayerIdByElementId(clines, elementLayerId);
    openLayerIds.insert(elementLayerId.begin(), elementLayerId.end());
    GetLayerIdByElementId(careas, elementLayerId);
    openLayerIds.insert(elementLayerId.begin(), elementLayerId.end());
}

void ElementsReportDialogViewCtrl::GetLayerIdByElementId(const std::vector< DataObjectId >& elementId,
                                                         std::set< DataObjectId >& elementLayerIds)
{
    for (const auto& element : elementId) {
        std::vector< DesignDatabase::DataObjectId > layerIds;
        uint32_t elementRet = LayerInterface::GetInst().GetElementAllLayers(element, layerIds);
        if (elementRet != ErrorCode::RET_OK) {
            PERROR(elementRet, "Failed to get element layers.")
            continue;
        }
        for (const auto& layerId : layerIds) {
            DesignDatabase::DataObjectId conLayerId = LayerInterface::GetInst().GetStackupId(layerId);
            std::vector< DesignDatabase::DataObjectId > subLayers;
            uint32_t subIdRet = Constraint::GetAllSubIdByTypePhyId(conLayerId, subLayers);
            if (subIdRet != ErrorCode::RET_OK) {
                PERROR(subIdRet, "Failed to get all sub layer id .")
                continue;
            }
            elementLayerIds.insert(subLayers.begin(), subLayers.end());
        }
    }
}

void ElementsReportDialogViewCtrl::GetAllDrcPinViaLayerId(
    const std::vector< DataObjectId >& violationIds,
    const std::vector< DesignDatabase::DataObjectId >& allVisibleLayers,
    std::set< DataObjectId >& openLayerIds)
{
    for (const auto& eleId : violationIds) {
        DataDict::DataObjType elementType = ElementBase::GetEleType(eleId);
        if (elementType == DataDict::DATA_PIN || elementType == DataDict::DATA_VIA) {
            GetPinViaConnectRelationLayer(eleId, allVisibleLayers, openLayerIds);
        } else if (elementType == DataDict::DATA_CONSTAINT_GROUP) {  // pinpair
            std::vector< DataObjectId > pins;
            if (Constraint::GetPinIdByPinpairId(eleId, pins) != ErrorCode::RET_OK) {
                PERROR(ErrorCode::RET_FAILURE, "Get pinId by pinpairId Failed!")
                continue;
            }
            for (const auto& pin : pins) {
                GetPinViaConnectRelationLayer(pin, allVisibleLayers, openLayerIds);
            }
        } else {
            GetOtherElementLayerId(eleId, openLayerIds);
        }
    }
}

void ElementsReportDialogViewCtrl::GetPinViaConnectRelationLayer(
    DesignDatabase::DataObjectId eleId,
    const std::vector< DataObjectId >& allVisibleLayers,
    std::set< DesignDatabase::DataObjectId >& elementLayerIds)
{
    std::vector< ConnectRelaInfo > infoElements;
    if (FindConnectRelation(eleId, infoElements) != ErrorCode::RET_OK) {
        PERROR(ErrorCode::RET_FAILURE, "Find Connect Relation Fail!")
        return;
    }
    std::vector< DataObjectId > elementIds;
    for (const auto& info : infoElements) {
        DataDict::DataObjType elementType = ElementBase::GetEleType(info.target);
        if (elementType == DataDict::DATA_CLINE || elementType == DataDict::DATA_COPPERAREA) {
            elementIds.emplace_back(info.target);
        }
    }
    if (elementIds.empty() && allVisibleLayers.empty()) {
        elementIds.emplace_back(eleId);
    }

    GetLayerIdByElementId(elementIds, elementLayerIds);
}

void ElementsReportDialogViewCtrl::GetOtherElementLayerId(DataObjectId eleId, std::set< DataObjectId >& elementLayerIds)
{
    DataDict::DataObjType elementType = ElementBase::GetEleType(eleId);
    if (elementType == DataDict::DATA_NCAREA || elementType == DataDict::DATA_TEXT ||
        elementType == DataDict::DATA_NCLINE) {
        std::vector< DesignDatabase::DataObjectId > layerIds;
        uint32_t ret = LayerInterface::GetInst().GetElementAllLayers(eleId, layerIds);
        if (ret != ErrorCode::RET_OK) {
            PERROR(ret, "Get element layers failed.")
            return;
        }
        elementLayerIds.insert(layerIds.begin(), layerIds.end());
    } else {
        std::vector< DataObjectId > elementId {eleId};
        GetLayerIdByElementId(elementId, elementLayerIds);
    }
}
void GetPinViaRelationLayer(const std::string& drcLayerName,
                            DataDict::DataObjType elementType,
                            std::set< DesignDatabase::DataObjectId >& elementLayerIds)
{
    std::string layerName = LayerKeyWords::LAYER_VIA + "/" + drcLayerName;
    if (elementType == DataDict::DATA_PIN) {
        layerName = LayerKeyWords::LAYER_PIN + "/" + drcLayerName;
    }
    elementLayerIds.insert(LayerInterface::GetInst().GetLayerId(layerName));
}
void ElementsReportDialogViewCtrl::GetNotAllDrcElementLayer(const std::string& drcLayerName,
                                                            const std::vector< DataObjectId >& violationIds,
                                                            std::set< DataObjectId >& elementLayerIds)
{
    for (const auto& eleId : violationIds) {
        DataDict::DataObjType elementType = ElementBase::GetEleType(eleId);
        if (elementType == DataDict::DATA_PIN || elementType == DataDict::DATA_VIA) {
            GetPinViaRelationLayer(drcLayerName, elementType, elementLayerIds);
        } else {
            std::vector< DesignDatabase::DataObjectId > layerIds;
            uint32_t ret = LayerInterface::GetInst().GetElementAllLayers(eleId, layerIds);
            if (ret != ErrorCode::RET_OK) {
                PERROR(ret, "Get element layers failed.")
                return;
            }
            elementLayerIds.insert(layerIds.begin(), layerIds.end());
        }
    }
}

void ElementsReportDialogViewCtrl::HighLightConsNetName(DataObjectId drcId)
{
    vector< uint64_t > drcVioId {};
    uint32_t ret = Constraint::GetDrcVioRelativedItems(drcId, drcVioId);
    if (ret != ErrorCode::RET_OK) {
        PERROR(ErrorCode::RET_FAILURE, "failed to get the drcvio ele id.")
        return;
    }
    QVector< uint64_t > netsVec {};
    for (auto objId : drcVioId) {
        if (objId == 0) {
            continue;
        }
        DataObjectId netId = 0;
        GetNetIdByObjId(objId, netId);
        if (netId == 0) {
            PWARNING(ErrorCode::RET_NOT_EXIST, "failed to get netId.")
            continue;
        }
        std::string netName;
        ret = GetNetNameById(netId, netName);
        if (ret != ErrorCode::RET_OK) {
            PERROR(ErrorCode::RET_FAILURE, "failed to get netName by netId.")
            return;
        }
        DataObjectId groupId = 0;
        ret                  = Constraint::GetConsGroupIdByName(ConstraintHierarcy::NET, netName, groupId);
        if (ret != ErrorCode::SUCCESS) {
            PERROR(ErrorCode::FAILURE, " get consgroup id is error.")
            return;
        }
        netsVec.push_back(groupId);
    }
    emit ViewCtrlEmitter::GetInstance().SendNetsNameToRulesSg(netsVec);
}

void ElementsReportDialogViewCtrl::GetNetIdByObjId(uint64_t objId, uint64_t& netId)
{
    DataObject* obj = DesignDatabase::DesignDBInterface::GetDataObjectRO(objId);
    if (obj == nullptr) {
        PERROR(ErrorCode::RET_FAILURE, "Get GetDataObjectRO failed!");
        return;
    }
    uint8_t doType = obj->GetDataObjType();
    switch (doType) {
        case DataDict::DATA_CLINE: {
            CLine* cline = static_cast< CLine* >(obj);
            netId        = cline->GetNetId();
            break;
        }
        case DataDict::DATA_VIA: {
            Via* via = static_cast< Via* >(obj);
            netId    = via->GetNetId();
            break;
        }
        case DataDict::DATA_PIN: {
            Pin* pin = static_cast< Pin* >(obj);
            netId    = pin->GetNetId();
            break;
        }
        case DataDict::DATA_COPPERAREA: {
            CArea* area = static_cast< CArea* >(obj);
            netId       = area->GetNetId();
            break;
        }
        case DataDict::DATA_HATCHLINE: {
            HatchLine* hatchLine = static_cast< HatchLine* >(obj);
            netId                = hatchLine->GetNetId();
            break;
        }
        default: {
            PWARNING(ErrorCode::RET_FAILURE, "Get GetDataObjectRO failed!");
            return;
        }
    }
}

bool ElementsReportDialogViewCtrl::GetElementDigStatus()
{
    if (GetLastDialog(dialogList_) == nullptr) {
        return false;
    } else {
        if (GetLastDialog(dialogList_)->isHidden()) {
            return false;
        }
    }
    return true;
}

void ElementsReportDialogViewCtrl::ExpandNetRectByRect(const std::string& netName, Rect& showRect)
{
    DataObjectId dataId = 0;
    uint32_t rtn        = LayoutEng::GetNetIdByName(netName, dataId);
    if (rtn != ErrorCode::RET_OK) {
        PERROR(rtn, "failed to get net id by net name.")
        return;
    }
    /* get the device bounding box using an algorithm */
    Rect rectTemp;
    rtn = LayoutEng::GetBoundingBoxByNet(dataId, rectTemp);
    if (rtn != ErrorCode::RET_OK) {
        PERROR(rtn, "failed to get bounding box of the net.")
        return;
    }
    showRect.ExpandRectByRect(rectTemp);
}

void ElementsReportDialogViewCtrl::LockElement(const string& netName)
{
    vector< vector< DesignDatabase::DataObjectId > > vecList;
    uint32_t ret = GetLockInfo(netName, vecList);
    if (ret != ErrorCode::RET_OK) {
        PERROR(ret, "Get lock Info error.")
        return;
    }
    for (auto it : vecList) {
        emit LayoutCmdEngine::CmdNotifyInterface::GetInstance().LockElementSg(it);
    }
}

void ElementsReportDialogViewCtrl::UnlockElement(const string& netName)
{
    vector< vector< DesignDatabase::DataObjectId > > vecList;
    uint32_t ret = GetLockInfo(netName, vecList);
    if (ret != ErrorCode::RET_OK) {
        PERROR(ret, "Get lock Info error.")
        return;
    }
    for (auto it : vecList) {
        emit LayoutCmdEngine::CmdNotifyInterface::GetInstance().UnlockElementSg(it);
    }
}

uint32_t ElementsReportDialogViewCtrl::GetLockInfo(const string& netName,
                                                   vector< vector< DesignDatabase::DataObjectId > >& vecList)
{
    DataObjectId dataId = 0;
    uint32_t rtn        = LayoutEng::GetNetIdByName(netName, dataId);
    if (rtn != ErrorCode::RET_OK) {
        PERROR(rtn, "failed to get net id by net name.")
        return ErrorCode::RET_FAILURE;
    }
    vector< uint64_t > pins;
    vector< uint64_t > vias;
    vector< uint64_t > lines;
    vector< uint64_t > areas;
    vector< DataObjectId > guidelines;
    rtn = LayoutEng::GetNetInfoById(dataId, pins, vias, lines, areas, guidelines);
    if (rtn != ErrorCode::RET_OK) {
        PERROR(rtn, "failed to get net info by net id.")
        return ErrorCode::RET_FAILURE;
    }
    if (pins.empty() && vias.empty() && lines.empty() && areas.empty()) {
        return ErrorCode::RET_FAILURE;
    }
    vecList.push_back(pins);
    vecList.push_back(vias);
    vecList.push_back(lines);
    vecList.push_back(areas);

    return ErrorCode::RET_OK;
}

QRectF ElementsReportDialogViewCtrl::ShowRectInCentre(const Rect& showRect)
{
    /* get the top right and bottom left points of the rectangle */
    qreal spX = static_cast< qreal >(showRect.GetRightTop().GetX());
    qreal spY = static_cast< qreal >(showRect.GetRightTop().GetY());
    QPointF startpos(spX, spY);
    qreal epX = static_cast< qreal >(showRect.GetLeftBottom().GetX());
    qreal epY = static_cast< qreal >(showRect.GetLeftBottom().GetY());
    QPointF endpos(epX, epY);
    double scale = StorageAccScaleToDou();
    int per      = static_cast< int >(scale);
    QRectF rectf(startpos / per, endpos / per);

    return rectf;
}

void ElementsReportDialogViewCtrl::HighlightPinpair(DesignDatabase::DataObjectId pinpairId)
{
    std::vector< DesignDatabase::DataObjectId > connectItems {};
    uint32_t rtn = ErrorCode::RET_OK;
    rtn          = Constraint::GetPinPairConnectionList(pinpairId, connectItems);
    if (rtn != ErrorCode::RET_OK) {
        PERROR(ErrorCode::RET_FAILURE, "failed to get connection list.")
        return;
    }
    if (connectItems.size() == 0) {
        return;
    }
    rtn = LayoutEng::SetHighlight(connectItems);
    if (rtn != ErrorCode::RET_OK) {
        PERROR(ErrorCode::RET_FAILURE, "failed to hightlight the pinpair.")
        return;
    }
}

void ElementsReportDialogViewCtrl::HighlightNet(const std::vector< std::string >& netNames, bool status)
{
    DataObjectId dataId = 0;
    vector< uint64_t > allItemId;
    CmdId cmdId = LayoutCmdEngine::CmdInterface::GetInstance().GetCurrentCmd();
    for (uint32_t i = 0; i < netNames.size(); ++i) {
        if (cmdId == CmdId::CMD_HILIGHT) {
            continue;
        }
        uint32_t rtn = LayoutEng::GetNetIdByName(netNames.at(i), dataId);
        if (rtn != ErrorCode::RET_OK) {
            PERROR(rtn, "failed to get net id by net name.")
            return;
        }
        Net* net = GET_DO_RO(LayoutEng::Net, dataId);
        if (net == nullptr) {
            PERROR(ErrorCode::RET_FAILURE, "net nullptr", dataId);
            return;
        }
        net->GetPins(allItemId);
        net->GetVias(allItemId);
        net->GetCAreas(allItemId);
        net->GetClines(allItemId);
        if (status) {
            net->GetGuildClines(allItemId);
        }
    }
    if (allItemId.empty()) {
        return;
    }
    uint32_t rtn = ErrorCode::RET_OK;
    rtn          = LayoutEng::SetHighlight(allItemId);
    if (rtn != ErrorCode::RET_OK) {
        PERROR(ErrorCode::RET_FAILURE, "failed to hightlight the net.")
        return;
    }
}

void ElementsReportDialogViewCtrl::GetAndShowNetsInfo(const std::vector< std::string >& netNames)
{
    if (netNames.empty()) {
        return;
    }
    vector< DataObjectId > netIds = GetNetIds(netNames);
    vector< std::map< LayoutEng::FindType,
                      std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >
        netsInfo {};
    if (LayoutEng::GetEleInfo(netIds, netsInfo) != ErrorCode::RET_OK) {
        PWARNING(ErrorCode::RET_FAILURE, "Failed to obtain nets information");
        return;
    }
    if (!netsInfo.empty()) {
        ShowInfoDialog(netsInfo);
    }
}

void ElementsReportDialogViewCtrl::HighlightDrcVio(DataObjectId DrcId)
{
    vector< uint64_t > drcVioId;
    uint32_t ret = Constraint::GetDrcVioRelativedItems(DrcId, drcVioId);
    if (ret != ErrorCode::RET_OK) {
        PERROR(ErrorCode::RET_FAILURE, "failed to get the drcvio ele id.")
        return;
    }
    drcVioId.push_back(DrcId);
    ret = LayoutEng::SetHighlight(drcVioId);
    if (ret != ErrorCode::RET_OK) {
        PERROR(ErrorCode::RET_FAILURE, "failed to hightlight the drcvio.")
    }
}

vector< DataObjectId > ElementsReportDialogViewCtrl::GetNetIds(const vector< std::string >& netNames)
{
    vector< DataObjectId > rst {};
    DataObjectId dataId = 0;
    string netName      = "";
    for (uint32_t i = 0; i < netNames.size(); ++i) {
        netName = netNames.at(i);
        if (LayoutEng::GetNetIdByName(netName, dataId) != ErrorCode::RET_OK) {
            continue;
        }
        rst.push_back(dataId);
    }
    return rst;
}

void ElementsReportDialogViewCtrl::ProcessCliCmd(const string& optName, const string& value)
{
    auto it = funcMap_.find(optName);
    if (it == funcMap_.end()) {
        return;
    }
    FuncDef fn = funcMap_[optName];
    (this->*fn)(value);
}

void ElementsReportDialogViewCtrl::ProcessHilightCmd(const std::vector< std::string >& netNames)
{
    CmdId cmdId = LayoutCmdEngine::CmdInterface::GetInstance().GetCurrentCmd();
    if (cmdId == CmdId::CMD_HILIGHT || cmdId == CmdId::CMD_DEHILIGHT) {
        vector< DataObjectId > eleVec;
        for (auto& it : netNames) {
            DataObjectId dataId = 0;
            uint32_t rtn        = LayoutEng::GetNetIdByName(it, dataId);
            if (rtn != ErrorCode::RET_OK) {
                PERROR(rtn, "failed to get net id by net name.")
                return;
            }
            eleVec.push_back(dataId);
        }
        emit CmdNotifyInterface::GetInstance().SelectElementSg(eleVec);
    }
}

void ElementsReportDialogViewCtrl::CloseElementsReportDialog(uint32_t index)
{
    ElementsReportDialog* lastDialog = GetLastDialog(dialogList_);
    if (lastDialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Last dialog is nullptr.")
        return;
    }
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr.")
        return;
    }
    popUpDlgPos_ = dialog->pos();
    if (index == lastDialog->GetDialogIndex()) {
        lastDialogIsClose_ = true;
    }
    if (dialogList_.size() > INDEX_IS_ONE) {
        for (auto dialogIter = dialogList_.begin(); dialogIter != dialogList_.end(); dialogIter++) {
            if (*dialogIter == dialog) {
                dialogIter = dialogList_.erase(dialogIter);  // 对话框释放后,删除list中的指针元素
                DeleteDialogAllInfo(index);
                RELEASE_DIALOG(dialog)
                break;
            }
        }
        // 如果关闭的是最后一个窗口，则将栈顶的前一个dialog的isNeedCreateDialog_置为true，表示下次info时需要打开新窗口
        if (index == lastDialog->GetDialogIndex()) {
            if (GetLastDialog(dialogList_) == nullptr) {
                PERROR(ErrorCode::RET_NULL_PTR, "Last dialog is nullptr.")
                return;
            }
            GetLastDialog(dialogList_)->SetNeedCreateDialogStatus(true);
        }
    } else if (dialogList_.size() == INDEX_IS_ONE) {
        // 当只有一个info窗口时与之前没有fix实现保持一致,因此列表中要保留一个info窗口
        if (dialog->GetDialogFixedStatus()) {
            dialog->SetDialogFixedStatus(false);  // 只有一个窗口且关闭则再次打开的info窗口为未固定状态
            dialog->SetNeedCreateDialogStatus(false);
        }
    }
    RecordingCloseDialog();
}

void ElementsReportDialogViewCtrl::GetTypeListCount(
    uint32_t index,
    const std::vector< std::map< LayoutEng::FindType,
                                 std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        values)
{
    infoListNumber_.clear();
    for (auto& allClassValue : values) {
        for (auto& oneClassValue : allClassValue) {
            GetTypeCount(oneClassValue.first, oneClassValue.second);
        }
    }
    for (auto& it : dialogInfoListNumber_) {  // 更新dialogInfoListNumber_数据
        if (it.first == index) {
            it.second = infoListNumber_;
            return;
        }
    }
    dialogInfoListNumber_.push_back(make_pair(index, infoListNumber_));
}

void ElementsReportDialogViewCtrl::DoWithNormalsInfo(const LayoutEng::ShowInfos::InfoKeyWords& keyWords,
                                                     const QString& data,
                                                     QString& text)
{
    if (!data.isEmpty()) {
        if (data.contains("\n")) {  //处理包含换行符
            bool isHaveKeyWords {true};
            if (!infoKeyWords_[keyWords].empty()) {
                AddSpaceRowData(text, QString::fromStdString(infoKeyWords_[keyWords]) + " :", "");
                isHaveKeyWords = true;
            } else {
                isHaveKeyWords = false;
            }
            SetNormalsHtmlText(isHaveKeyWords, data, text);
        } else {
            if (!infoKeyWords_[keyWords].empty()) {
                AddSpaceRowData(text, QString::fromStdString(infoKeyWords_[keyWords]) + " :", data);
            } else {
                AddSpaceRowData(text, "", data);
            }
        }
    }
}

void ElementsReportDialogViewCtrl::SetNormalsHtmlText(bool isHaveKeyWords, const QString& data, QString& text)
{
    QStringList propertyInfoList = data.split("\n");
    for (int i = 0; i < propertyInfoList.size(); i++) {
        if (i == propertyInfoList.size() - INDEX_IS_ONE) {
            MakeHtmlText(text, isHaveKeyWords == true ? TransStartText(propertyInfoList.at(i))
                                                      : TransNoKeyStartText(propertyInfoList.at(i)));
        } else {
            MakeHtmlText(text, isHaveKeyWords == true ? TransStartText(propertyInfoList.at(i)) + "<br>"
                                                      : TransNoKeyStartText(propertyInfoList.at(i)) + "<br>");
        }
    }
}

void ElementsReportDialogViewCtrl::DoWithIncludePointInfo(
    const LayoutEng::ShowInfos::InfoKeyWords& keyWords,
    const std::vector< LayoutEng::ShowInfos::InfoChildData >& info,
    QString& text)
{
    bool isHaveCoreWords {true};
    if (!infoKeyWords_[keyWords].empty()) {
        isHaveCoreWords = true;
        AddSpaceRowData(text, QString::fromStdString(infoKeyWords_[keyWords]) + " :", "");
    } else {
        isHaveCoreWords = false;
    }
    // 处理含坐标碎片信息的 值信息
    int count    = 0;
    int infoSize = static_cast< int >(info.size());
    for (auto infoChildData : info) {
        if (!infoChildData.data.empty()) {
            count++;
            if (infoChildData.id == 0) {  // 该碎片信息不是坐标,不需要跳转
                NoPointShardsInfo(infoChildData, infoSize, count, isHaveCoreWords, text);
            } else {  //该碎片信息是坐标,需要跳转
                PointShardsInfo(infoChildData, infoSize, count, isHaveCoreWords, text);
            }
        }
    }
}

void ElementsReportDialogViewCtrl::NoPointShardsInfo(const LayoutEng::ShowInfos::InfoChildData& infoChildData,
                                                     const int& infoSize,
                                                     const int& count,
                                                     const bool& isHaveKeyWords,
                                                     QString& text)
{
    if (infoChildData.infoFlag == LayoutEng::ShowInfos::InfoFlag::IS_END) {
        MakeHtmlText(text, (count == infoSize) ? QString::fromStdString(infoChildData.data)
                                               : TransTextAndEnd(QString::fromStdString(infoChildData.data)));
    } else if (infoChildData.infoFlag == LayoutEng::ShowInfos::InfoFlag::IS_START) {
        MakeHtmlText(text, isHaveKeyWords == true ? TransStartText(QString::fromStdString(infoChildData.data))
                                                  : TransNoKeyStartText(QString::fromStdString(infoChildData.data)));
    } else {
        MakeHtmlText(text, QString::fromStdString(infoChildData.data));
    }
}
void ElementsReportDialogViewCtrl::PointShardsInfo(const LayoutEng::ShowInfos::InfoChildData& infoChildData,
                                                   const int& infoSize,
                                                   const int& count,
                                                   const bool& isHaveKeyWords,
                                                   QString& text)
{
    if (infoChildData.infoFlag == LayoutEng::ShowInfos::InfoFlag::IS_END) {
        MakeHtmlText(
            text, (count == infoSize)
                      ? TransHyperLink("elem:", '(' + QString::fromStdString(infoChildData.data) + ')',
                                       QString::number(infoChildData.id))
                      : TransTextAndEnd(TransHyperLink("elem:", '(' + QString::fromStdString(infoChildData.data) + ')',
                                                       QString::number(infoChildData.id))));
    } else if (infoChildData.infoFlag == LayoutEng::ShowInfos::InfoFlag::IS_START) {

        MakeHtmlText(
            text,
            isHaveKeyWords == true
                ? TransStartText(TransHyperLink("elem:", '(' + QString::fromStdString(infoChildData.data) + ')',
                                                QString::number(infoChildData.id)))
                : TransNoKeyStartText(TransHyperLink("elem:", '(' + QString::fromStdString(infoChildData.data) + ')',
                                                     QString::number(infoChildData.id))));
    } else {
        MakeHtmlText(text, TransHyperLink("elem:", '(' + QString::fromStdString(infoChildData.data) + ')',
                                          QString::number(infoChildData.id)));
    }
}

void ElementsReportDialogViewCtrl::DoWithPinListValue(const LayoutEng::ShowInfos::InfoKeyWords& keyWords,
                                                      const std::vector< LayoutEng::ShowInfos::InfoChildData >& info,
                                                      QString& text)
{
    AddSpaceRowData(text, QString::fromStdString(infoKeyWords_[keyWords]) + " :", "");
    for (auto infoChildData : info) {
        if (!infoChildData.data.empty()) {  // PinList一共两列信息，IS_START不是坐标，IS_END是坐标
            if (infoChildData.infoFlag == LayoutEng::ShowInfos::InfoFlag::IS_START) {  // 不跳转
                MakeHtmlText(text, TransPinListStart(QString::fromStdString(infoChildData.data)));
            } else if (infoChildData.infoFlag == LayoutEng::ShowInfos::InfoFlag::IS_END) {  // 跳转
                MakeHtmlText(text, TransPinListEnd(
                                       TransHyperLink("elem:", '(' + QString::fromStdString(infoChildData.data) + ')',
                                                      QString::number(infoChildData.id))));
            }
        }
    }
}

void ElementsReportDialogViewCtrl::DoWithNormalPointInfo(const LayoutEng::ShowInfos::InfoKeyWords& keyWords,
                                                         const QString& data,
                                                         const DesignDatabase::DataObjectId& id,
                                                         QString& text)
{
    if (!data.isNull() && id != 0) {
        AddSpaceRowData(text, QString::fromStdString(infoKeyWords_[keyWords]) + " :",
                        TransHyperLink("elem:", '(' + data + ')', QString::number(id)));
    }
}

void ElementsReportDialogViewCtrl::DoWithElectricalInfo(const QString& data, QString& text)
{
    if (!data.isEmpty()) {
        if (data.contains("\n")) {  //处理包含换行符
            QStringList propertyInfoList = data.split("\n");
            for (int i = 0; i < propertyInfoList.size(); i++) {
                SetElectricalHtmlText(i, propertyInfoList, text);
            }
        }
    }
}

void ElementsReportDialogViewCtrl::SetElectricalHtmlText(int i, QStringList& propertyInfoList, QString& text)
{
    if (i == 0) {
        MakeHtmlText(text, TransNoKeyStartText(propertyInfoList.at(i)) + "<br>");
    } else {
        if (i == propertyInfoList.size() - INDEX_IS_ONE) {
            MakeHtmlText(text, TransStartText(propertyInfoList.at(i)));
        } else {
            MakeHtmlText(text, TransStartText(propertyInfoList.at(i)) + "<br>");
        }
    }
}

uint32_t ElementsReportDialogViewCtrl::SplittingLink(const QUrl& link,
                                                     DesignDatabase::DataObjectId& id,
                                                     QStringList& point)
{
    QString str  = link.toString();
    QString doId = str.mid((str.lastIndexOf("doId:") + INDEX_IS_FIVE));
    if (doId.isEmpty()) {
        PERROR(ErrorCode::RET_FAILURE, "This doId is empty!")
        return ErrorCode::RET_FAILURE;
    }
    id = doId.toULongLong();
    if (!DO_IS_VALID(id)) {
        return ErrorCode::RET_FAILURE;
    }
    QStringList pathList = str.split(")");
    if (pathList.size() != INDEX_IS_TWO) {
        PERROR(ErrorCode::RET_FAILURE, "The size of pathList not equal to two!")
        return ErrorCode::RET_FAILURE;
    }
    QString pointString = pathList[0].mid((pathList[0].lastIndexOf("elem:(") + INDEX_IS_SIX));
    if (pointString.isEmpty()) {
        PERROR(ErrorCode::RET_FAILURE, "This point is empty!")
        return ErrorCode::RET_FAILURE;
    }
    point = pointString.split(" ");
    if (point.size() != INDEX_IS_TWO) {
        PERROR(ErrorCode::RET_FAILURE, "The size of point not equal to two!")
        return ErrorCode::RET_FAILURE;
    }
    return ErrorCode::RET_OK;
}

void ElementsReportDialogViewCtrl::HighLight(const QUrl& link)
{
    DesignDatabase::DataObjectId id {};
    QStringList point {};
    uint32_t splitRet = SplittingLink(link, id, point);
    if (splitRet != ErrorCode::RET_OK) {
        PERROR(splitRet, "Failed to split the link!")
        return;
    }
    std::vector< DesignDatabase::DataObjectId > ids;
    ids.push_back(id);
    DataDict::DataObjType elementType = ElementBase::GetEleType(id);
    // traces, lines, trace segs,other segs,areas,guidelines 使用点跳转 DTS2024022612405 DTS2024051312858
    if (find(CENTER_POINT_TYPE.begin(), CENTER_POINT_TYPE.end(), elementType) != CENTER_POINT_TYPE.end()) {
        uint32_t clearRet = ClearHighlightedElements();
        if (clearRet != ErrorCode::RET_OK) {
            PWARNING(clearRet, "Failed to clear hilight elements.");
            return;
        }
        uint32_t setRet = LayoutEng::SetHighlight(ids);
        if (setRet != ErrorCode::RET_OK) {
            PERROR(setRet, "Failed to hilight elements.");
            return;
        }
        QPointF pointF = QPointF(point[0].toDouble(), point[INDEX_IS_ONE].toDouble());
        ViewWindowTabViewCtrl::GetInst().SetCenterPos(pointF);
    } else {  // 其他的使用自身跳转
        uint32_t ret = ZoomInInterface::GetInst().ZoomInHightLight(ids, false);
        if (ret != ErrorCode::SUCCESS) {
            return;
        }
    }
}

void ElementsReportDialogViewCtrl::GetTypeValue(
    const LayoutEng::FindType& type,
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values,
    QString& text)
{
    if (switchValueFuncMap_.find(type) == switchValueFuncMap_.end()) {
        return;
    }
    valueFuncDef fn = switchValueFuncMap_[type];
    (this->*fn)(values, text);
}

void ElementsReportDialogViewCtrl::MakeHtmlText(QString& htmlText, const QString& addText)
{

    if (htmlText.size() > TEXT_HTML_MAX_SIZE) {
        return;
    }
    htmlText += addText;
}

void ElementsReportDialogViewCtrl::GetTypeCount(
    const LayoutEng::FindType& type,
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& infoData)
{
    if (switchCountFuncMap_.find(type) == switchCountFuncMap_.end()) {
        return;
    }
    typeFuncDef fn = switchCountFuncMap_[type];
    (this->*fn)(infoData);
}

bool ElementsReportDialogViewCtrl::AreaIsBoundaryLayer(
    const std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData >& values)
{
    auto it = values.find(LayoutEng::ShowInfos::LAYER_INFO);
    if (it == values.end()) {
        PERROR(ErrorCode::RET_FAILURE, "Failed to get area layer info");
        return true;
    }
    bool flag = false;
    for (auto layer : it->second.info) {
        if (QString::fromStdString(layer.data).contains("Boundary")) {
            flag = true;
            break;
        }
    }
    if (flag && !CommonSw::GetInstance().IsShowBoundaryEnable()) {  // 默认不显示Boundary/Top信息,命令行可开启查看
        return true;
    }
    return false;
}

void ElementsReportDialogViewCtrl::ClassifiedDataByType(
    const std::vector< std::map< LayoutEng::FindType,
                                 std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        allInfo,
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoMapByType)
{
    infoMapByType.clear();
    for (auto& it : allInfo) {
        infoMapByType[it.begin()->first].push_back(it.begin()->second);
    }
}

void ElementsReportDialogViewCtrl::ElementTypeChange(const FindType& currentType, uint32_t index)
{
    ClearDisplayCount(index);
    IntialSliceData(index);
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >
        infoMapByType;
    GetInfoMapByType(index, infoMapByType);
    // 如果当前显示的类型是铜皮或挖空，且sw_cfg show_info_boundary off（不显示boundary信息），则先剔除boundary信息
    if (currentType == AREAS || currentType == CUTOUTS_CAVITIES) {
        if (!CommonSw::GetInstance().IsShowBoundaryEnable()) {
            SortOutBoundaryInfo(currentType, infoMapByType);
            SetInfoMapByType(index, infoMapByType);
        }
    }
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr.")
        return;
    }
    dialog->SetPageLeftBtnEnableStatus(false);
    dialog->SetPageRightBtnEnableStatus(false);
    QString onceHtml = GetNextDataForShowOnce(index, currentType, infoMapByType, true);
    if (onceHtml.isEmpty()) {
        MsgInterface::GetInstance().ShowInfoToConsole("Want to view the boundary information, "
                                                      "please input the command : sw_cfg show_info_boundary on/off");
    }
    dialog->SetOnceHtml(onceHtml);
}

QString ElementsReportDialogViewCtrl::GetCountHtmlForShowFirst(uint32_t index, const FindType& currentType)
{
    QString countHtml = "";
    if (ITEM_TYPE_MAP.find(currentType) == ITEM_TYPE_MAP.end()) {
        PERROR(ErrorCode::RET_NOT_EXIST, "info data err.")
        return countHtml;
    }
    for (auto& it : dialogInfoListNumber_) {
        if (it.first == index) {
            if (it.second[currentType] > 0) {
                QString listingStr = "Listing : ";
                QString numberStr = QString::number(it.second[currentType]) + " elements " + ITEM_TYPE_MAP[currentType];
                countHtml         = "<div><td width=230>" + listingStr + "</td>" + "<td>" + numberStr + "</td></div>";
            }
            break;
        }
    }
    return countHtml;
}

void ElementsReportDialogViewCtrl::UpdateNextPageData(LayoutEng::FindType currentType, uint32_t index)
{
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >
        infoMapByType;
    GetInfoMapByType(index, infoMapByType);
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString onceHtml             = GetNextDataForShowOnce(index, currentType, infoMapByType);
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr.")
        return;
    }
    dialog->SetOnceHtml(onceHtml);
    QApplication::restoreOverrideCursor();
}

QString ElementsReportDialogViewCtrl::GetNextDataForShowOnce(
    uint32_t index,
    const FindType& currentType,
    const std::map< LayoutEng::FindType,
                    std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoMapByType,
    bool isNewOnceDisplay)
{
    auto it = infoMapByType.find(currentType);
    if (it == infoMapByType.end()) {
        PERROR(ErrorCode::RET_NOT_EXIST, "info data err.")
        return "";
    }
    QString onceHtml  = "";
    QString countHtml = "";
    if (IsDisplayCountDataForRight(index)) {
        countHtml = GetCountHtmlForShowFirst(index, currentType);
        onceHtml += countHtml;
    }
    // 表示当前显示的是超100万的超大字符串信息的一部分，下一页需要接着显示改字符串
    if (!GetSliceData(index).empty() && GetCurrentSliceNumber(index) < GetSliceData(index).size() - INDEX_IS_ONE) {
        return GetNextSliceData(static_cast< uint32_t >(it->second.size()), index, onceHtml);
    }
    uint32_t lastDisplayCount  = GetLastDisplayCount(index);
    uint32_t firstDisplayCount = lastDisplayCount == 0 ? lastDisplayCount : lastDisplayCount - INDEX_IS_ONE;
    SetFirstDisplayCount(index, firstDisplayCount);
    for (uint32_t i = GetLastDisplayCount(index); i < it->second.size(); i++) {
        IntialSliceData(index);
        QString tempHtml = "";
        GetTypeValue(currentType, it->second[i], tempHtml);
        if (onceHtml.size() + tempHtml.size() > MAX_DISPLAYED_CHARACTERS) {
            if (tempHtml.size() > MAX_DISPLAYED_CHARACTERS) {  // 超100万的超大字符串
                return GetNextOnceHtml(index, static_cast< uint32_t >(it->second.size()), isNewOnceDisplay, tempHtml,
                                       countHtml, onceHtml);
            } else {
                break;
            }
        }
        onceHtml += tempHtml;
        SetLastDisplayCount(index, i + INDEX_IS_ONE);
    }
    SetPageBtnEnableAfterTurnRight(index, GetLastDisplayCount(index), static_cast< uint32_t >(it->second.size()),
                                   isNewOnceDisplay);
    return onceHtml;
}

QString ElementsReportDialogViewCtrl::GetNextOnceHtml(
    uint32_t index, uint32_t size, bool isNewOnceDisplay, QString& tempHtml, QString& countHtml, QString& onceHtml)
{
    if (!onceHtml.isEmpty() && onceHtml != countHtml) {
        SetPageBtnEnableAfterTurnRight(index, GetLastDisplayCount(index) - INDEX_IS_ONE, size, isNewOnceDisplay);
        return onceHtml;  //如果下一个显示的对象是超大的，那先把再次之前拼装好的字符串返回显示，超大对象的信息在下一页显示
    }
    std::vector< QString > sliceDatas = GetSliceData(index);
    SliceData(index, tempHtml, sliceDatas);  // 分片
    SetAllSliceDatas(index, sliceDatas);
    onceHtml += sliceDatas[0];
    SetPageBtnEnableAfterTurnRight(index, GetLastDisplayCount(index), size, isNewOnceDisplay);
    return onceHtml;
}

QString ElementsReportDialogViewCtrl::GetNextSliceData(const uint32_t& allElementCount,
                                                       uint32_t index,
                                                       QString& onceHtml)
{
    SetFirstDisplayCount(index, GetLastDisplayCount(index));
    SetCurrentSliceNumber(index, GetCurrentSliceNumber(index) + INDEX_IS_ONE);
    if (GetCurrentSliceNumber(index) >= GetSliceData(index).size()) {
        PERROR(ErrorCode::RET_INVALID, "currentSliceNumber is error.")
        return "";
    }
    onceHtml += GetSliceData(index)[GetCurrentSliceNumber(index)];
    SetPageBtnEnableAfterTurnRight(index, GetLastDisplayCount(index), allElementCount);
    if (GetCurrentSliceNumber(index) == GetSliceData(index).size() - INDEX_IS_ONE) {
        SetLastDisplayCount(index, GetLastDisplayCount(index) + INDEX_IS_ONE);
    }
    return onceHtml;
}

void ElementsReportDialogViewCtrl::SliceData(uint32_t index,
                                             const QString& targetHtml,
                                             std::vector< QString >& sliceDatas)
{
    if (targetHtml.size() >
        // 该分支返回的是，100万个字符再加上到第一个换行符位置处的字符,为了避免一行数据被截断
        MAX_DISPLAYED_CHARACTERS) {  // = MAX_DISPLAYED_CHARACTERS(100万), 走else
        QString frontHalf     = targetHtml.mid(0, MAX_DISPLAYED_CHARACTERS);
        QString backHalf      = targetHtml.mid(MAX_DISPLAYED_CHARACTERS);
        int newLineCharNumber = backHalf.indexOf("<br>");
        QString partHtml      = backHalf.left(newLineCharNumber);
        sliceDatas.push_back(frontHalf + partHtml);
        SliceData(index, backHalf.mid(newLineCharNumber), sliceDatas);
    } else {
        if (!targetHtml.isEmpty()) {
            sliceDatas.push_back(targetHtml);
        }
        return;
    }
}

void ElementsReportDialogViewCtrl::UpdatePreviousPageData(FindType currentType, uint32_t index)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >
        infoMapByType;
    GetInfoMapByType(index, infoMapByType);
    QString onceHtml             = GetPreviousDataForShowOnce(currentType, infoMapByType, index);
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr.")
        return;
    }
    dialog->SetOnceHtml(onceHtml);
    QApplication::restoreOverrideCursor();
}

QString ElementsReportDialogViewCtrl::GetPreviousDataForShowOnce(
    const LayoutEng::FindType& currentType,
    const std::map< LayoutEng::FindType,
                    std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoMapByType,
    uint32_t index)
{
    auto it = infoMapByType.find(currentType);
    if (it == infoMapByType.end()) {
        PERROR(ErrorCode::RET_NOT_EXIST, "info data err.")
        return "";
    }
    QString onceHtml = "";
    // 表示当前显示的是超100万的超大字符串信息的一部分，下一页需要接着显示改字符串
    if (!GetSliceData(index).empty() && GetCurrentSliceNumber(index) > 0) {
        return GetPreviousSliceData(currentType, index, onceHtml);
    }
    SetLastDisplayCount(index, GetFirstDisplayCount(index) + INDEX_IS_ONE);
    for (int i = static_cast< int >(GetFirstDisplayCount(index)); i >= 0; i--) {
        IntialSliceData(index);
        QString tempHtml = "";
        if (static_cast< uint32_t >(i) >= it->second.size()) {
            continue;
        }
        GetTypeValue(currentType, it->second[static_cast< uint32_t >(i)], tempHtml);
        if (onceHtml.size() + tempHtml.size() > MAX_DISPLAYED_CHARACTERS) {
            if (tempHtml.size() > MAX_DISPLAYED_CHARACTERS) {  // 超100万的超大字符串
                return GetPreviousOnceHtml(index, currentType, tempHtml, onceHtml);
            } else {
                break;
            }
        }
        onceHtml         = tempHtml + onceHtml;
        int displayCount = i == 0 ? 0 : i - INDEX_IS_ONE;
        SetFirstDisplayCount(index, static_cast< uint32_t >(displayCount));
    }
    SetPageBtnEnableAfterTurnLeft(index);
    if (IsDisplayCountDataForLeft(index)) {
        QString countHtml = GetCountHtmlForShowFirst(index, currentType);
        onceHtml          = countHtml + onceHtml;
    }
    return onceHtml;
}

QString ElementsReportDialogViewCtrl::GetPreviousOnceHtml(uint32_t index,
                                                          const LayoutEng::FindType& currentType,
                                                          QString& tempHtml,
                                                          QString& onceHtml)
{
    if (!onceHtml.isEmpty()) {
        SetPageBtnEnableAfterTurnLeft(index);
        return onceHtml;  //如果下一个显示的对象是超大的，那先把再次之前拼装好的字符串返回显示，超大对象的信息在下一页显示
    }
    std::vector< QString > sliceDatas = GetSliceData(index);
    SliceData(index, tempHtml, sliceDatas);  // 分片
    SetAllSliceDatas(index, sliceDatas);
    onceHtml += sliceDatas[0];
    SetPageBtnEnableAfterTurnLeft(index);
    if (IsDisplayCountDataForLeft(index)) {
        QString countHtml = GetCountHtmlForShowFirst(index, currentType);
        onceHtml          = countHtml + onceHtml;
    }
    if (GetFirstDisplayCount(index) > 0) {
        SetFirstDisplayCount(index, GetFirstDisplayCount(index) - INDEX_IS_ONE);
    }
    if (GetFirstDisplayCount(index) == 0 && !GetSliceData(index).empty()) {
        SetLastDisplayCount(index, 0);
    }
    return onceHtml;
}

QString ElementsReportDialogViewCtrl::GetPreviousSliceData(const LayoutEng::FindType& currentType,
                                                           uint32_t index,
                                                           QString& onceHtml)
{
    SetCurrentSliceNumber(index, GetCurrentSliceNumber(index) - INDEX_IS_ONE);
    if (GetCurrentSliceNumber(index) >= GetSliceData(index).size()) {
        PERROR(ErrorCode::RET_INVALID, "currentSliceNumber is error.")
        return "";
    }
    onceHtml += GetSliceData(index)[GetCurrentSliceNumber(index)];
    SetPageBtnEnableAfterTurnLeft(index);
    if (IsDisplayCountDataForLeft(index)) {
        QString countHtml = GetCountHtmlForShowFirst(index, currentType);
        onceHtml          = countHtml + onceHtml;
    }
    if (GetCurrentSliceNumber(index) == 0) {
        if (GetFirstDisplayCount(index) > 0) {
            SetFirstDisplayCount(index, GetFirstDisplayCount(index) - INDEX_IS_ONE);
        }
        if (GetLastDisplayCount(index) > 0 && GetFirstDisplayCount(index) == 0) {
            SetLastDisplayCount(index, GetLastDisplayCount(index) - INDEX_IS_ONE);
        }
    }
    return onceHtml;
}

void ElementsReportDialogViewCtrl::SetPageBtnEnableAfterTurnLeft(uint32_t index)
{

    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr!")
        return;
    }
    if (GetFirstDisplayCount(index) == 0) {
        if (GetCurrentSliceNumber(index) == 0) {
            dialog->SetPageLeftBtnEnableStatus(false);
        } else {
            dialog->SetPageLeftBtnEnableStatus(true);
        }
    } else {
        dialog->SetPageLeftBtnEnableStatus(true);
    }
    dialog->SetPageRightBtnEnableStatus(true);
}

void ElementsReportDialogViewCtrl::SetPageBtnEnableAfterTurnRight(uint32_t index,
                                                                  const uint32_t lastDisplayCount,
                                                                  const uint32_t allElementCount,
                                                                  bool isNewOnceDisplay)
{
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr.")
        return;
    }
    // DTS2024070818578
    if (lastDisplayCount == INDEX_IS_ZERO && allElementCount == INDEX_IS_ZERO) {
        dialog->SetPageLeftBtnEnableStatus(false);
        dialog->SetPageRightBtnEnableStatus(false);
        return;
    }
    std::vector< QString > sliceData = GetSliceData(index);
    if (lastDisplayCount >= allElementCount - INDEX_IS_ONE) {
        if (GetCurrentSliceNumber(index) == sliceData.size() - INDEX_IS_ONE || sliceData.empty()) {
            dialog->SetPageRightBtnEnableStatus(false);
        } else {
            dialog->SetPageRightBtnEnableStatus(true);
        }
    } else {
        dialog->SetPageRightBtnEnableStatus(true);
    }
    dialog->SetPageLeftBtnEnableStatus(true);
    if (isNewOnceDisplay) {
        dialog->SetPageLeftBtnEnableStatus(false);
    }
}

void ElementsReportDialogViewCtrl::SetAllTypeValuesForSave(
    const std::vector< std::map< LayoutEng::FindType,
                                 std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        values,
    uint32_t index)
{
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr.")
        return;
    }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    std::map< LayoutEng::FindType, QString > textValue {};
    InitTextValue(index, textValue);
    for (auto allClassValue : values) {
        for (auto oneClassValue : allClassValue) {
            GetTypeValue(oneClassValue.first, oneClassValue.second, textValue[oneClassValue.first]);
        }
    }
    QApplication::restoreOverrideCursor();
    dialog->ExportFile(textValue);
}

void ElementsReportDialogViewCtrl::SaveInfoFile(uint32_t index)
{
    std::vector< std::map< LayoutEng::FindType,
                           std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >
        dialogAllInfo;
    for (auto& it : dialogAllInfo_) {
        if (it.first == index) {
            dialogAllInfo = it.second;
            break;
        }
    }
    SetAllTypeValuesForSave(dialogAllInfo, index);
}

void ElementsReportDialogViewCtrl::IntialSliceData(uint32_t index)
{
    SetCurrentSliceNumber(index, 0);
    std::vector< QString > sliceDatas {};
    SetAllSliceDatas(index, sliceDatas);
}

bool ElementsReportDialogViewCtrl::IsDisplayCountDataForRight(uint32_t index)
{
    if (GetLastDisplayCount(index) == 0) {
        if (!GetSliceData(index).empty()) {
            return false;
        }
        return true;
    }
    return false;
}

bool ElementsReportDialogViewCtrl::IsDisplayCountDataForLeft(uint32_t index)
{
    if (GetFirstDisplayCount(index) == 0) {
        if (!GetSliceData(index).empty() && GetCurrentSliceNumber(index) != 0) {
            return false;
        }
        return true;
    }
    return false;
}

void ElementsReportDialogViewCtrl::SortOutBoundaryInfo(
    const LayoutEng::FindType& currentType,
    std::map< FindType, std::vector< std::map< ShowInfos::InfoKeyWords, ShowInfos::InfoData > > >& infoMapByType)
{
    auto it = infoMapByType.find(currentType);
    if (it == infoMapByType.end()) {
        PERROR(ErrorCode::RET_NOT_EXIST, "info data err, No area or cutout information exists.")
        return;
    }
    for (auto iter = it->second.begin(); iter != it->second.end();) {
        QString tempHtml = "";
        GetTypeValue(currentType, *iter, tempHtml);
        if (tempHtml.isEmpty()) {
            iter = it->second.erase(iter);
        } else {
            iter++;
        }
    }
}

ElementsReportDialog* ElementsReportDialogViewCtrl::GetDialogByIndex(uint32_t index)
{
    auto it = dialogList_.begin();
    while (it != dialogList_.end()) {
        if ((*it)->GetDialogIndex() == index) {
            return (*it);
        }
        ++it;
    }
    return nullptr;
}

uint32_t ElementsReportDialogViewCtrl::GetLastDialogIndex(const std::list< ElementsReportDialog* >& dialogList)
{
    if (dialogList.empty()) {
        return 0;
    }
    if (dialogList.back() == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Last dialog is nullptr!")
        return 0;
    }
    return dialogList.back()->GetDialogIndex();
}

ElementsReportDialog* ElementsReportDialogViewCtrl::GetLastDialog(const std::list< ElementsReportDialog* >& dialogList)
{
    if (dialogList.empty()) {
        return nullptr;
    }
    return dialogList.back();
}

void ElementsReportDialogViewCtrl::RecordingCloseDialog()
{
    // record command into script file
    std::string cmdInfo = CMD_NAME_OPTION_CFG + " " + LayoutGui::ELEMENTS_REPORT_CFG_CLOSE;
    emit ViewCtrlEmitter::GetInstance().RecordCmdSg(cmdInfo);
}

void ElementsReportDialogViewCtrl::NewElementsReportDialog(ElementsReportDialog* dialog)
{
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr!")
        return;
    }
    dialog->SetDialogIndex(GetLastDialogIndex(dialogList_) + INDEX_IS_ONE);
    InitalSliceData(dialog->GetDialogIndex());
    GetTypeListCount(dialog->GetDialogIndex(), allInfo_);
    dialogList_.push_back(dialog);
    dialogAllInfo_.push_back(make_pair(GetLastDialogIndex(dialogList_), allInfo_));
    dialog->InitInfoKeyWords(infoKeyWords_);
    dialog->Initial();
    InitialConnect(dialog);
    InitToSetData(dialog);
}
void ElementsReportDialogViewCtrl::InitToSetData(ElementsReportDialog* dialog)
{
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr!")
        return;
    }
    uint32_t index = dialog->GetDialogIndex();
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >
        infoMapByType;
    for (auto& it : dialogAllInfo_) {
        if (it.first == index) {
            ClassifiedDataByType(it.second, infoMapByType);
            uint32_t updateMapRet = UpdateAllInfoMapByType(index, infoMapByType, it.second);
            if (updateMapRet == ErrorCode::RET_OK) {  // 更新成功，不需要插入新数据
                return;
            } else if (updateMapRet == ErrorCode::RET_FAILURE) {  // 更新失败，需要插入新数据
                allInfoMapByType_.push_back(make_pair(index, infoMapByType));
                dialog->SetPageLeftBtnEnableStatus(false);
                dialog->SetPageRightBtnEnableStatus(false);
                dialog->SetValue(it.second);  //只填充表格的model
                break;
            } else {
                PERROR(ErrorCode::RET_FAILURE, "Failed to update or insert the allInfoMapByType!")
                return;
            }
        }
    }
}

uint32_t ElementsReportDialogViewCtrl::UpdateAllInfoMapByType(
    uint32_t index,
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoMapByType,
    std::vector< std::map< LayoutEng::FindType,
                           std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >& allInfo)
{
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "This dialog is nullptr!")
        return ErrorCode::RET_NULL_PTR;
    }
    for (auto& iter : allInfoMapByType_) {  // 更新allInfoMapByType_数据
        if (iter.first == index) {
            iter.second = infoMapByType;
            dialog->SetPageLeftBtnEnableStatus(false);
            dialog->SetPageRightBtnEnableStatus(false);
            dialog->SetValue(allInfo);  //只填充表格的model
            return ErrorCode::RET_OK;
        }
    }
    return ErrorCode::RET_FAILURE;
}

void ElementsReportDialogViewCtrl::ClickFnCf()
{
    InitialCliFnCf();
    connect(&CliNotifyInterface::GetInstance(), &CliNotifyInterface::OptionCfgSg, this,
            &ElementsReportDialogViewCtrl::ProcessCliCmd, Qt::UniqueConnection);
}

void ElementsReportDialogViewCtrl::RecordDlgFixedStatus(bool isDialogFixed, uint32_t index)
{
    ElementsReportDialog* dialog = GetDialogByIndex(index);
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr.")
        return;
    }
    // 只录制新弹出的info窗口的固定，未固定状态（因为旧的窗口固不固定没有什么作用）
    if (dialog == GetLastDialog(dialogList_)) {
        std::string fixedStatus;
        if (isDialogFixed) {
            fixedStatus = "fixed";
        } else {
            fixedStatus = "unfixed";
        }
        // record command into script file
        std::string cmdInfo = CMD_NAME_OPTION_CFG + " " + CMD_NAME_INFO_FIXED_STATUS + " ";
        emit ViewCtrlEmitter::GetInstance().RecordCmdSg(cmdInfo + fixedStatus);
    }
}

void ElementsReportDialogViewCtrl::UpdateDialogAllInfo()
{
    if (GetLastDialog(dialogList_) == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Last dialog is nullptr!")
        return;
    }
    uint32_t lastDlgIndex = GetLastDialog(dialogList_)->GetDialogIndex();
    for (auto& it : dialogAllInfo_) {
        if (it.first == lastDlgIndex) {
            it.second = allInfo_;
            break;
        }
    }
}

void ElementsReportDialogViewCtrl::DeleteDialogAllInfo(uint32_t index)
{
    for (auto it = dialogAllInfo_.begin(); it != dialogAllInfo_.end(); it++) {
        if ((*it).first == index) {
            dialogAllInfo_.erase(it);
            break;
        }
    }
    for (auto it = dialogInfoListNumber_.begin(); it != dialogInfoListNumber_.end(); it++) {
        if ((*it).first == index) {
            dialogInfoListNumber_.erase(it);
            break;
        }
    }
    for (auto it = allInfoMapByType_.begin(); it != allInfoMapByType_.end(); it++) {
        if ((*it).first == index) {
            allInfoMapByType_.erase(it);
            break;
        }
    }
}

void ElementsReportDialogViewCtrl::MoveLastDialogPos()
{
    if (GetLastDialog(dialogList_) == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Last dialog is nullptr!")
        return;
    }
    // DTS2023102714306
    if (lastDialogIsClose_) {  // 关闭:窗口位置为上次关闭的位置
        GetLastDialog(dialogList_)->showNormal();
        GetLastDialog(dialogList_)->move(popUpDlgPos_.x(), popUpDlgPos_.y());
        lastDialogIsClose_ = false;
    } else {  //窗口最小化:窗口为最小化时的位置
        QPoint currentDlgPos = GetLastDialog(dialogList_)->pos();
        GetLastDialog(dialogList_)->showNormal();
        GetLastDialog(dialogList_)->move(currentDlgPos.x(), currentDlgPos.y());
    }
}

void ElementsReportDialogViewCtrl::SetInfoMapByType(
    uint32_t index,
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoMapByType)
{
    auto iter = allInfoMapByType_.begin();
    while (iter != allInfoMapByType_.end()) {
        if ((*iter).first == index) {
            (*iter).second = infoMapByType;
            break;
        }
        ++iter;
    }
    if (iter == allInfoMapByType_.end()) {
        PERROR(ErrorCode::RET_FAILURE, "allInfoMapByType not found. The index of dialog is ", index)
    }
}

void ElementsReportDialogViewCtrl::GetInfoMapByType(
    uint32_t index,
    std::map< LayoutEng::FindType,
              std::vector< std::map< LayoutEng::ShowInfos::InfoKeyWords, LayoutEng::ShowInfos::InfoData > > >&
        infoMapByType)
{
    for (auto& it : allInfoMapByType_) {
        if (it.first == index) {
            infoMapByType = it.second;
            break;
        }
    }
}

void ElementsReportDialogViewCtrl::CloseAllDialog()
{
    if (!dialogList_.empty()) {
        for (auto& dialogIter : dialogList_) {
            RELEASE_DIALOG(dialogIter)
        }
    }
    dialogList_.clear();
    // 清除所有成员变量
    lastDialogIsClose_ = false;
    popUpDlgPos_       = {0, 0};
    dialogAllInfo_.clear();
    dialogInfoListNumber_.clear();
    allInfoMapByType_.clear();
    allSliceDatas_.clear();
    allCurrentSliceNumber_.clear();
    allLastDisplayCount_.clear();
    allFirstDisplayCount_.clear();
}

void ElementsReportDialogViewCtrl::MoveNewDialogPos(ElementsReportDialog* dialog)
{
    if (dialog == nullptr) {
        PERROR(ErrorCode::RET_NULL_PTR, "Dialog is nullptr!")
        return;
    }
    dialog->showNormal();                              // DTS2023120104639 解决最小化后不弹窗问题
    dialog->move(popUpDlgPos_.x(), popUpDlgPos_.y());  // info每次打开窗口位置为上一次关闭info窗口的位置；
}

void ElementsReportDialogViewCtrl::SetAllSliceDatas(uint32_t index, std::vector< QString >& sliceDatas)
{
    auto iter = allSliceDatas_.begin();
    while (iter != allSliceDatas_.end()) {
        if ((*iter).first == index) {
            (*iter).second = sliceDatas;
            break;
        }
        ++iter;
    }
    if (iter == allSliceDatas_.end()) {
        PERROR(ErrorCode::RET_FAILURE, "sliceDatas not found. The index of dialog is ", index)
    }
}

std::vector< QString > ElementsReportDialogViewCtrl::GetSliceData(uint32_t index)
{
    std::vector< QString > sliceData {};
    for (auto& iter : allSliceDatas_) {
        if (iter.first == index) {
            return iter.second;
        }
    }
    return sliceData;
}

void ElementsReportDialogViewCtrl::SetCurrentSliceNumber(uint32_t index, uint32_t currentSliceNumber)
{
    auto iter = allCurrentSliceNumber_.begin();
    while (iter != allCurrentSliceNumber_.end()) {
        if ((*iter).first == index) {
            (*iter).second = currentSliceNumber;
            break;
        }
        ++iter;
    }
    if (iter == allCurrentSliceNumber_.end()) {
        PERROR(ErrorCode::RET_FAILURE, "allCurrentSliceNumber not found. The index of dialog is ", index)
    }
}

uint32_t ElementsReportDialogViewCtrl::GetCurrentSliceNumber(uint32_t index)
{
    for (auto& iter : allCurrentSliceNumber_) {
        if (iter.first == index) {
            return iter.second;
        }
    }
    return 0;
}

void ElementsReportDialogViewCtrl::ClearDisplayCount(uint32_t index)
{
    for (auto& it : allLastDisplayCount_) {
        if (it.first == index) {
            it.second = 0;
            break;
        }
    }
    for (auto& iter : allFirstDisplayCount_) {
        if (iter.first == index) {
            iter.second = 0;
            break;
        }
    }
}

uint32_t ElementsReportDialogViewCtrl::GetFirstDisplayCount(uint32_t index)
{
    for (auto& iter : allFirstDisplayCount_) {
        if (iter.first == index) {
            return iter.second;
        }
    }
    return 0;
}

uint32_t ElementsReportDialogViewCtrl::GetLastDisplayCount(uint32_t index)
{
    for (auto& iter : allLastDisplayCount_) {
        if (iter.first == index) {
            return iter.second;
        }
    }
    return 0;
}

void ElementsReportDialogViewCtrl::SetFirstDisplayCount(uint32_t index, uint32_t displayCount)
{
    auto iter = allFirstDisplayCount_.begin();
    while (iter != allFirstDisplayCount_.end()) {
        if ((*iter).first == index) {
            (*iter).second = displayCount;
            break;
        }
        ++iter;
    }
    if (iter == allFirstDisplayCount_.end()) {
        PERROR(ErrorCode::RET_FAILURE, "FirstDisplayCount not found. The index of dialog is ", index)
    }
}

void ElementsReportDialogViewCtrl::SetLastDisplayCount(uint32_t index, uint32_t displayCount)
{
    auto iter = allLastDisplayCount_.begin();
    while (iter != allLastDisplayCount_.end()) {
        if ((*iter).first == index) {
            (*iter).second = displayCount;
            break;
        }
        ++iter;
    }
    if (iter == allLastDisplayCount_.end()) {
        PERROR(ErrorCode::RET_FAILURE, "LastDisplayCount not found. The index of dialog is ", index)
    }
}

void ElementsReportDialogViewCtrl::InitalSliceData(uint32_t index)
{
    allFirstDisplayCount_.push_back(make_pair(index, 0));
    allLastDisplayCount_.push_back(make_pair(index, 0));
    std::vector< QString > sliceDatas {};
    allSliceDatas_.push_back(make_pair(index, sliceDatas));
    allCurrentSliceNumber_.push_back(make_pair(index, 0));
}
