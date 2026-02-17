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
    return Oper->getOperandValue(2);
}

    OpResult getY() {
    return Oper->getResultValue(0);
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
    return Oper->getOperandValue(2);
}

    OpResult getY() {
    return Oper->getResultValue(0);
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

OpResult getIndices() {
    return Oper->getResultValue(1);
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
