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

def generate_header
    <<~HEADER
    #include "graph/Op.hpp"
    #include <memory>
    #include <string>
    namespace GiantGraph {

    HEADER
end

def generate_footer
    <<~FOOTER
    } // namespace GiantGraph
    FOOTER
end

def generate_input(input, index)
    name = input["name"]
    <<~CPP
    OpOperand get#{name}() {
        return Oper->getOperandValue(#{index});
    }
    CPP
end

def generate_output(output, index)
    name = output["name"]
    <<~CPP
    OpResult get#{name}() {
        return Oper->getResultValue(#{index});
    }
  CPP
end


def generate_attr(attr, op_name)
    # name = attr["name"]
    # type = YAML_TO_CPP_TYPESHIT[attr["type"]]
    # if (attr.key?("default"))
        # formatted_val = format_value(attr["default"])
#     <<~CPP
#         #{type} #{name}() const  {
#             auto it = node_->attrs_.find("#{name}");
#             if (it == node_->attrs_.end()) {
#
#             std::cout << "Got #{name} attr" << " by default." << std::endl;
#                 return #{type}(#{formatted_val});
#             }
#
#             std::cout << "Got #{name} attr." << std::endl;
#             return std::get<#{type}>(it->second);
#         }
#     CPP
#     else
#     <<~CPP
#         #{type} #{name}() const  {
#             auto it = node_->attrs_.find("#{name}");
#             assert(it == node_->attrs_.end());
#             std::cout << "Got #{name} attr" << std::endl;
#             return std::get<#{type}>(it->second);
#         }
#     CPP
    # end
end

def generate_op(op)

    name = op["name"]

    inputs = []
    if op["inputs"]
        inputs = op["inputs"].each_with_index.map {|input, index| generate_input(input, index)}
    end

    outputs = []
    if op["outputs"]
        outputs = op["outputs"].each_with_index.map {|output, index| generate_output(output, index)}
    end

    attrs = []
    if op["attributes"]
        attrs = op["attributes"].map { |attr| generate_attr(attr, name) }
    end

    <<~CPP
    struct Op#{name} {
        explicit #{name}(Operation* Oper) : Op<#{name}>(Oper) {}
        ~#{name} = default;

        static std::string getOperationName() {
            return "#{name}"
        }

        #{inputs.join("\n")}
        #{outputs.join("\n")}
        #{attrs.join("\n")}

        bool verify() const {
            if (Oper->getName() != "#{name}") {
                return false;
            }

            if (Oper->getNumOperands() != #{inputs.length}) {
                return false;
            }

            if (Oper->getNumResults() != #{outputs.length}) {
                return false;
            }

            return true;
        }
    }
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
