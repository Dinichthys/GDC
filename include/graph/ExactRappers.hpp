#pragma once
#include <optional>
#include <memory>
#include <string>
#include "graph/Op.hpp"
namespace GiantGraph {

struct OpConv : Op<OpConv>{
    using Op<OpConv>::Op; // inheriting constructor
    ~OpConv() = default;

    static std::string getOperationName() {
        return "Conv";
    }

    OpOperand getInput() const {
        return Oper->getOperandValue(0);
    }

    OpOperand getFilter() const {
        return Oper->getOperandValue(1);
    }

    std::optional<OpOperand> getBias() const {
        return Oper->getNumOperands() > 2 ?
            std::optional(Oper->getOperandValue(2)) : std::nullopt;
    }

    OpResult getOutput() const {
        return Oper->getResultValue(0);
    }

    std::optional<std::vector<int64_t>> getKernelShape() const  {
        auto it = Oper->findAttribute("kernel_shape");
        if (it != Oper->attributesEnd()) {
            std::cout << "Got kernel_shape attr" << std::endl;
            return it->second.get<std::vector<int64_t>>();
        }
        return std::nullopt;
    }

    int64_t getGroup() const  {
        auto it = Oper->findAttribute("group");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got group attr" << " by default." << std::endl;
            return int64_t(1);
        }

        std::cout << "Got group attr." << std::endl;
        return it->second.get<int64_t>();
    }

    std::vector<int64_t> getDilations() const  {
        auto it = Oper->findAttribute("dilations");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got dilations attr" << " by default." << std::endl;
            return std::vector<int64_t>({1, 1});
        }

        std::cout << "Got dilations attr." << std::endl;
        return it->second.get<std::vector<int64_t>>();
    }

    std::vector<int64_t> getPads() const  {
        auto it = Oper->findAttribute("pads");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got pads attr" << " by default." << std::endl;
            return std::vector<int64_t>({0, 0});
        }

        std::cout << "Got pads attr." << std::endl;
        return it->second.get<std::vector<int64_t>>();
    }

    std::vector<int64_t> getStrides() const  {
        auto it = Oper->findAttribute("strides");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got strides attr" << " by default." << std::endl;
            return std::vector<int64_t>({1, 1});
        }

        std::cout << "Got strides attr." << std::endl;
        return it->second.get<std::vector<int64_t>>();
    }


    bool verify() const {
        if (Oper->getName() != "Conv") {
            return false;
        }

        if (Oper->getNumOperands() != 2) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
};
struct OpGemm : Op<OpGemm>{
    using Op<OpGemm>::Op; // inheriting constructor
    ~OpGemm() = default;

    static std::string getOperationName() {
        return "Gemm";
    }

    OpOperand getLhs() const {
        return Oper->getOperandValue(0);
    }

    OpOperand getRhs() const {
        return Oper->getOperandValue(1);
    }

    std::optional<OpOperand> getBias() const {
        return Oper->getNumOperands() > 2 ?
            std::optional(Oper->getOperandValue(2)) : std::nullopt;
    }

    OpResult getOutput() const {
        return Oper->getResultValue(0);
    }

    float getAlpha() const  {
        auto it = Oper->findAttribute("alpha");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got alpha attr" << " by default." << std::endl;
            return float(1.0f);
        }

        std::cout << "Got alpha attr." << std::endl;
        return it->second.get<float>();
    }

    float getBeta() const  {
        auto it = Oper->findAttribute("beta");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got beta attr" << " by default." << std::endl;
            return float(1.0f);
        }

        std::cout << "Got beta attr." << std::endl;
        return it->second.get<float>();
    }

    int64_t getTransa() const  {
        auto it = Oper->findAttribute("transA");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got transA attr" << " by default." << std::endl;
            return int64_t(0);
        }

        std::cout << "Got transA attr." << std::endl;
        return it->second.get<int64_t>();
    }

    int64_t getTransb() const  {
        auto it = Oper->findAttribute("transB");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got transB attr" << " by default." << std::endl;
            return int64_t(0);
        }

        std::cout << "Got transB attr." << std::endl;
        return it->second.get<int64_t>();
    }


    bool verify() const {
        if (Oper->getName() != "Gemm") {
            return false;
        }

        if (Oper->getNumOperands() != 2) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
};
struct OpAdd : Op<OpAdd>{
    using Op<OpAdd>::Op; // inheriting constructor
    ~OpAdd() = default;

    static std::string getOperationName() {
        return "Add";
    }

    OpOperand getLhs() const {
        return Oper->getOperandValue(0);
    }

    OpOperand getRhs() const {
        return Oper->getOperandValue(1);
    }

    OpResult getOutput() const {
        return Oper->getResultValue(0);
    }



    bool verify() const {
        if (Oper->getName() != "Add") {
            return false;
        }

        if (Oper->getNumOperands() != 2) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
};
struct OpRelu : Op<OpRelu>{
    using Op<OpRelu>::Op; // inheriting constructor
    ~OpRelu() = default;

    static std::string getOperationName() {
        return "Relu";
    }

    OpOperand getInput() const {
        return Oper->getOperandValue(0);
    }

    OpResult getOutput() const {
        return Oper->getResultValue(0);
    }



    bool verify() const {
        if (Oper->getName() != "Relu") {
            return false;
        }

        if (Oper->getNumOperands() != 1) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
};
struct OpMul : Op<OpMul>{
    using Op<OpMul>::Op; // inheriting constructor
    ~OpMul() = default;

    static std::string getOperationName() {
        return "Mul";
    }

    OpOperand getLhs() const {
        return Oper->getOperandValue(0);
    }

    OpOperand getRhs() const {
        return Oper->getOperandValue(1);
    }

    OpResult getOutput() const {
        return Oper->getResultValue(0);
    }



    bool verify() const {
        if (Oper->getName() != "Mul") {
            return false;
        }

        if (Oper->getNumOperands() != 2) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
};
struct OpMatMul : Op<OpMatMul>{
    using Op<OpMatMul>::Op; // inheriting constructor
    ~OpMatMul() = default;

    static std::string getOperationName() {
        return "MatMul";
    }

    OpOperand getLhs() const {
        return Oper->getOperandValue(0);
    }

    OpOperand getRhs() const {
        return Oper->getOperandValue(1);
    }

    OpResult getOutput() const {
        return Oper->getResultValue(0);
    }



    bool verify() const {
        if (Oper->getName() != "MatMul") {
            return false;
        }

        if (Oper->getNumOperands() != 2) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
};
struct OpMaxPool : Op<OpMaxPool>{
    using Op<OpMaxPool>::Op; // inheriting constructor
    ~OpMaxPool() = default;

    static std::string getOperationName() {
        return "MaxPool";
    }

    OpOperand getInput() const {
        return Oper->getOperandValue(0);
    }

    OpResult getOutput() const {
        return Oper->getResultValue(0);
    }

    std::optional<OpResult> getIndices() const {
        return Oper->getNumResults() > 1 ?
            std::optional(Oper->getResultValue(1)) : std::nullopt;
    }

    std::optional<std::vector<int64_t>> getKernelShape() const  {
        auto it = Oper->findAttribute("kernel_shape");
        if (it != Oper->attributesEnd()) {
            std::cout << "Got kernel_shape attr" << std::endl;
            return it->second.get<std::vector<int64_t>>();
        }
        return std::nullopt;
    }

    std::string getAutoPad() const  {
        auto it = Oper->findAttribute("auto_pad");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got auto_pad attr" << " by default." << std::endl;
            return std::string("NOTSET");
        }

        std::cout << "Got auto_pad attr." << std::endl;
        return it->second.get<std::string>();
    }

    int64_t getCeilMode() const  {
        auto it = Oper->findAttribute("ceil_mode");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got ceil_mode attr" << " by default." << std::endl;
            return int64_t(0);
        }

        std::cout << "Got ceil_mode attr." << std::endl;
        return it->second.get<int64_t>();
    }

    std::vector<int64_t> getDilations() const  {
        auto it = Oper->findAttribute("dilations");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got dilations attr" << " by default." << std::endl;
            return std::vector<int64_t>({1, 1});
        }

        std::cout << "Got dilations attr." << std::endl;
        return it->second.get<std::vector<int64_t>>();
    }

    std::vector<int64_t> getPads() const  {
        auto it = Oper->findAttribute("pads");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got pads attr" << " by default." << std::endl;
            return std::vector<int64_t>({0, 0});
        }

        std::cout << "Got pads attr." << std::endl;
        return it->second.get<std::vector<int64_t>>();
    }

    int64_t getStorageOrder() const  {
        auto it = Oper->findAttribute("storage_order");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got storage_order attr" << " by default." << std::endl;
            return int64_t(0);
        }

        std::cout << "Got storage_order attr." << std::endl;
        return it->second.get<int64_t>();
    }

    std::vector<int64_t> getStrides() const  {
        auto it = Oper->findAttribute("strides");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got strides attr" << " by default." << std::endl;
            return std::vector<int64_t>({1, 1});
        }

        std::cout << "Got strides attr." << std::endl;
        return it->second.get<std::vector<int64_t>>();
    }


    bool verify() const {
        if (Oper->getName() != "MaxPool") {
            return false;
        }

        if (Oper->getNumOperands() != 1) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
};
} // namespace GiantGraph
