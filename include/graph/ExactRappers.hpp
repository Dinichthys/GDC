#include "graph/Op.hpp"
#include <memory>
#include <string>
namespace GiantGraph {

struct OpConv {
    explicit Conv(Operation* Oper) : Op<Conv>(Oper) {}
    ~Conv = default;

    static std::string getOperationName() {
        return "Conv"
    }

    OpOperand getX() {
    return Oper->getOperandValue(0);
}

OpOperand getW() {
    return Oper->getOperandValue(1);
}

OpOperand getB() {
    assert(Oper->getNumOperands() >= index);
    return Oper->getOperandValue(2);
}

    OpResult getY() {
    return Oper->getResultValue(0);
}

    std::vector<int64_t> kernel_shape() const  {
    auto it = findAttribute("kernel_shape");
    assert(it == attributesEnd());
    std::cout << "Got kernel_shape attr" << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}

 getGroup() const  {
    auto it = findAttribute("group");
    if (it == attributeEnd()) {

    std::cout << "Got group attr" << " by default." << std::endl;
        return (1);
    }

    std::cout << "Got group attr." << std::endl;
    return std::get<>(it->second);
}

std::vector<int64_t> getDilations() const  {
    auto it = findAttribute("dilations");
    if (it == attributeEnd()) {

    std::cout << "Got dilations attr" << " by default." << std::endl;
        return std::vector<int64_t>({1, 1});
    }

    std::cout << "Got dilations attr." << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}

std::vector<int64_t> getPads() const  {
    auto it = findAttribute("pads");
    if (it == attributeEnd()) {

    std::cout << "Got pads attr" << " by default." << std::endl;
        return std::vector<int64_t>({0, 0});
    }

    std::cout << "Got pads attr." << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}

std::vector<int64_t> getStrides() const  {
    auto it = findAttribute("strides");
    if (it == attributeEnd()) {

    std::cout << "Got strides attr" << " by default." << std::endl;
        return std::vector<int64_t>({1, 1});
    }

    std::cout << "Got strides attr." << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}


    bool verify() const {
        if (Oper->getName() != "Conv") {
            return false;
        }

        if (Oper->getNumOperands() != 3) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
}
struct OpGemm {
    explicit Gemm(Operation* Oper) : Op<Gemm>(Oper) {}
    ~Gemm = default;

    static std::string getOperationName() {
        return "Gemm"
    }

    OpOperand getA() {
    return Oper->getOperandValue(0);
}

OpOperand getB() {
    return Oper->getOperandValue(1);
}

OpOperand getC() {
    assert(Oper->getNumOperands() >= index);
    return Oper->getOperandValue(2);
}

    OpResult getY() {
    return Oper->getResultValue(0);
}

    float getAlpha() const  {
    auto it = findAttribute("alpha");
    if (it == attributeEnd()) {

    std::cout << "Got alpha attr" << " by default." << std::endl;
        return float(1.0f);
    }

    std::cout << "Got alpha attr." << std::endl;
    return std::get<float>(it->second);
}

float getBeta() const  {
    auto it = findAttribute("beta");
    if (it == attributeEnd()) {

    std::cout << "Got beta attr" << " by default." << std::endl;
        return float(1.0f);
    }

    std::cout << "Got beta attr." << std::endl;
    return std::get<float>(it->second);
}

int64_t getTransa() const  {
    auto it = findAttribute("transA");
    if (it == attributeEnd()) {

    std::cout << "Got transA attr" << " by default." << std::endl;
        return int64_t(0);
    }

    std::cout << "Got transA attr." << std::endl;
    return std::get<int64_t>(it->second);
}

int64_t getTransb() const  {
    auto it = findAttribute("transB");
    if (it == attributeEnd()) {

    std::cout << "Got transB attr" << " by default." << std::endl;
        return int64_t(0);
    }

    std::cout << "Got transB attr." << std::endl;
    return std::get<int64_t>(it->second);
}


    bool verify() const {
        if (Oper->getName() != "Gemm") {
            return false;
        }

        if (Oper->getNumOperands() != 3) {
            return false;
        }

        if (Oper->getNumResults() != 1) {
            return false;
        }

        return true;
    }
}
struct OpAdd {
    explicit Add(Operation* Oper) : Op<Add>(Oper) {}
    ~Add = default;

    static std::string getOperationName() {
        return "Add"
    }

    OpOperand getA() {
    return Oper->getOperandValue(0);
}

OpOperand getB() {
    return Oper->getOperandValue(1);
}

    OpResult getC() {
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
}
struct OpRelu {
    explicit Relu(Operation* Oper) : Op<Relu>(Oper) {}
    ~Relu = default;

    static std::string getOperationName() {
        return "Relu"
    }

    OpOperand getX() {
    return Oper->getOperandValue(0);
}

    OpResult getY() {
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
}
struct OpMul {
    explicit Mul(Operation* Oper) : Op<Mul>(Oper) {}
    ~Mul = default;

    static std::string getOperationName() {
        return "Mul"
    }

    OpOperand getA() {
    return Oper->getOperandValue(0);
}

OpOperand getB() {
    return Oper->getOperandValue(1);
}

    OpResult getC() {
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
}
struct OpMatMul {
    explicit MatMul(Operation* Oper) : Op<MatMul>(Oper) {}
    ~MatMul = default;

    static std::string getOperationName() {
        return "MatMul"
    }

    OpOperand getA() {
    return Oper->getOperandValue(0);
}

OpOperand getB() {
    return Oper->getOperandValue(1);
}

    OpResult getY() {
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
}
struct OpMaxPool {
    explicit MaxPool(Operation* Oper) : Op<MaxPool>(Oper) {}
    ~MaxPool = default;

    static std::string getOperationName() {
        return "MaxPool"
    }

    OpOperand getX() {
    return Oper->getOperandValue(0);
}

    OpResult getY() {
    return Oper->getResultValue(0);
}

OpOperand getIndices() {
    assert(Oper->getNumOperands() >= index);
    return Oper->getOperandValue(1);
}

    std::vector<int64_t> kernel_shape() const  {
    auto it = findAttribute("kernel_shape");
    assert(it == attributesEnd());
    std::cout << "Got kernel_shape attr" << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}

std::string getAuto_pad() const  {
    auto it = findAttribute("auto_pad");
    if (it == attributeEnd()) {

    std::cout << "Got auto_pad attr" << " by default." << std::endl;
        return std::string("NOTSET");
    }

    std::cout << "Got auto_pad attr." << std::endl;
    return std::get<std::string>(it->second);
}

int64_t getCeil_mode() const  {
    auto it = findAttribute("ceil_mode");
    if (it == attributeEnd()) {

    std::cout << "Got ceil_mode attr" << " by default." << std::endl;
        return int64_t(0);
    }

    std::cout << "Got ceil_mode attr." << std::endl;
    return std::get<int64_t>(it->second);
}

std::vector<int64_t> getDilations() const  {
    auto it = findAttribute("dilations");
    if (it == attributeEnd()) {

    std::cout << "Got dilations attr" << " by default." << std::endl;
        return std::vector<int64_t>({1, 1});
    }

    std::cout << "Got dilations attr." << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}

std::vector<int64_t> getPads() const  {
    auto it = findAttribute("pads");
    if (it == attributeEnd()) {

    std::cout << "Got pads attr" << " by default." << std::endl;
        return std::vector<int64_t>({0, 0});
    }

    std::cout << "Got pads attr." << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}

int64_t getStorage_order() const  {
    auto it = findAttribute("storage_order");
    if (it == attributeEnd()) {

    std::cout << "Got storage_order attr" << " by default." << std::endl;
        return int64_t(0);
    }

    std::cout << "Got storage_order attr." << std::endl;
    return std::get<int64_t>(it->second);
}

std::vector<int64_t> getStrides() const  {
    auto it = findAttribute("strides");
    if (it == attributeEnd()) {

    std::cout << "Got strides attr" << " by default." << std::endl;
        return std::vector<int64_t>({1, 1});
    }

    std::cout << "Got strides attr." << std::endl;
    return std::get<std::vector<int64_t>>(it->second);
}


    bool verify() const {
        if (Oper->getName() != "MaxPool") {
            return false;
        }

        if (Oper->getNumOperands() != 1) {
            return false;
        }

        if (Oper->getNumResults() != 2) {
            return false;
        }

        return true;
    }
}
} // namespace GiantGraph
