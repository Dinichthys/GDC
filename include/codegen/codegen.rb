YAML_PATH = "onnx.yaml"

require 'yaml'

YAML_TO_CPP_TYPESHIT = {
    "int64[]" => "std::vector<int64_t>",
    "int64" => "int64_t",
    "float32[]" => "std::vector<float>",
    "float32" => "float",
    "string" => "std::string"
}

def format_value(value)
    if value.is_a?(Array)
        "{#{value.join(", ")}}"
    elsif value.is_a?(Float)
        "#{value}f"
    else
        value
    end

end

def camelize(snake_case)
    snake_case.split('_').map(&:capitalize).join
end

def generate_header
    <<~HEADER
    #pragma once
    #include <optional>
    #include <memory>
    #include <string>
    #include "graph/Op.hpp"
    namespace GiantGraph {

    HEADER
end

def generate_footer
    <<~FOOTER
    } // namespace GiantGraph
    FOOTER
end

def count_required(io_list)
    io_list.count { |item| !item["optional"] }
end

def count_total(io_list)
    io_list.length
end

def generate_input(input, index)
    name = input["semantic"]
    if input.key?("optional")
<<-CPP
    std::optional<OpOperand> get#{camelize(name)}() const {
        return Oper->getNumOperands() > #{index} ?
            std::optional(Oper->getOperandValue(#{index})) : std::nullopt;
    }
    CPP
    else
<<-CPP
    OpOperand get#{camelize(name)}() const {
        return Oper->getOperandValue(#{index});
    }
    CPP
    end
end

def generate_output(output, index)
    name = output["semantic"]
    if output.key?("optional")
<<-CPP
    std::optional<OpResult> get#{camelize(name)}() const {
        return Oper->getNumResults() > #{index} ?
            std::optional(Oper->getResultValue(#{index})) : std::nullopt;
    }
    CPP
    else
<<-CPP
    OpResult get#{camelize(name)}() const {
        return Oper->getResultValue(#{index});
    }
    CPP
    end
end

def generate_attr(attr, op_name)
    name = attr["name"]
    type = YAML_TO_CPP_TYPESHIT[attr["type"]]
    if attr.key?("default")
        formatted_val = format_value(attr["default"])
<<-CPP
    #{type} get#{camelize(name)}() const  {
        auto it = Oper->findAttribute("#{name}");
        if (it == Oper->attributesEnd()) {

        std::cout << "Got #{name} attr" << " by default." << std::endl;
            return #{type}(#{formatted_val});
        }

        std::cout << "Got #{name} attr." << std::endl;
        return it->second.get<#{type}>();
    }
    CPP
    else
<<-CPP
    std::optional<#{type}> get#{camelize(name)}() const  {
        auto it = Oper->findAttribute("#{name}");
        if (it != Oper->attributesEnd()) {
            std::cout << "Got #{name} attr" << std::endl;
            return it->second.get<#{type}>();
        }
        return std::nullopt;
    }
    CPP
    end
end
def generate_op(op)

    name = op["name"]

    inputs = []
    if op["inputs"]
        inputs = op["inputs"].each_with_index.map {|input, index| generate_input(input, index)}
    end

    print inputs

    outputs = []
    if op["outputs"]
        outputs = op["outputs"].each_with_index.map {|output, index| generate_output(output, index)}
    end

    attrs = []
    if op["attributes"]
        attrs = op["attributes"].each_with_index.map { |attr| generate_attr(attr, name) }
    end

    <<~CPP
    struct Op#{name} : Op<Op#{name}>{
        using Op<Op#{name}>::Op; // inheriting constructor
        ~Op#{name}() = default;

        static std::string getOperationName() {
            return "#{name}";
        }

    #{inputs.join("\n")}
    #{outputs.join("\n")}
    #{attrs.join("\n")}

        bool verify() const {
            if (Oper->getName() != "#{name}") {
                return false;
            }

            if (Oper->getNumOperands() != #{count_required(inputs)}) {
                return false;
            }

            if (Oper->getNumResults() != #{count_required(outputs)}) {
                return false;
            }

            return true;
        }
    };
    CPP

end

def main

    ops = YAML.load_file(YAML_PATH)
    code_gen = File.open("../graph/ExactRappers.hpp", "w")

    code_gen.write(generate_header())

    ops.each do |op|
        code_gen.write(generate_op(op))
    end

    code_gen.write(generate_footer())

    puts "Generated #{ops.size} operations!"

end


main if __FILE__ == $0
