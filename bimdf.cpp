#include "bimdf.h"
#include "core/string/ustring.h"

void BIMDF::solve() {
        using BiMDF = Satsuma::BiMDF;
        BiMDF bimdf;
        std::map<std::string, BiMDF::Edge> edges;

        auto x = bimdf.add_node(); // boundary
        auto a = bimdf.add_node();
        auto b = bimdf.add_node();
        auto c = bimdf.add_node();

        using Abs = Satsuma::CostFunction::AbsDeviation;
        using Quad = Satsuma::CostFunction::QuadDeviation;
        using Zero = Satsuma::CostFunction::Zero;
        edges["x_x"] = bimdf.add_edge({
                .u = x,
                .v = x,
                .u_head = false,
                .v_head = false,
                .cost_function = Zero{},
        });
        edges["x_a"] = bimdf.add_edge({ .u = x, .v = a, .u_head = true, .v_head = true, .cost_function = Quad{ .target = 4, .weight = 1 }, .lower = 1 });
        edges["a_b"] = bimdf.add_edge({ .u = a, .v = b, .u_head = false, .v_head = false, .cost_function = Abs{ .target = .7, .weight = 1 }, .lower = 0 });
        edges["a_c"] = bimdf.add_edge({ .u = a, .v = c, .u_head = false, .v_head = false, .cost_function = Abs{ .target = .4, .weight = 1 }, .lower = 0 });
        edges["b_c"] = bimdf.add_edge({ .u = b, .v = c, .u_head = true, .v_head = true, .cost_function = Abs{ .target = .2, .weight = 1 }, .lower = 0 });

        auto config = Satsuma::BiMDFSolverConfig{
                .matching_solver = Satsuma::MatchingSolver::Lemon
        };
        auto result = Satsuma::solve_bimdf(bimdf, config);
        std::ostringstream buffer;
        buffer << "Total cost: " << result.cost << "\n";
        for (const auto& [name, edge] : edges) {
                buffer << "Flow on " << name << ": " << (*result.solution)[edge] << "\n";
        }
        buffer << result.stopwatch << std::endl;
        std::string output = buffer.str();
        print_line(output.c_str());
}
