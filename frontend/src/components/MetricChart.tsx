import {CartesianGrid, Line, LineChart, XAxis, YAxis} from "recharts"
import {
    Card,
    CardContent,
    CardDescription,
    CardHeader,
    CardTitle,
} from "@/components/ui/card"

import {
    type ChartConfig,
    ChartContainer,
    ChartTooltip,
    ChartTooltipContent,
} from "@/components/ui/chart"

import type {WeatherReading} from "@/services/types.ts";
import {Skeleton} from "@/components/ui/skeleton.tsx";

interface MetricChartProps {
    metric: 'temperature' | 'humidity' | 'pressure',
    data: WeatherReading[];
    isLoading?: boolean;
}

const metricChartConfigurations: ChartConfig = {
    temperature: {
        label: "Temperature (°C)",
        color: "hsl(var(--chart-1))",
    },

    humidity: {
        label: "Humidity (%)",
        color: "hsl(var(--chart-2))",
    },
    pressure: {
        label: "Pressure (hPa)",
        color: "hsl(var(--chart-3))",
    },
} satisfies ChartConfig;




const ChartSkeleton = () => (
    <Card>
        <CardHeader>
            <Skeleton className="h-5 w-32" />
            <Skeleton className="h-4 w-48" />
        </CardHeader>
        <CardContent>
            <div className="h-[250px] w-full space-y-3">
                <div className="space-y-2">
                    <Skeleton className="h-[250px] w-full px-6" />
                </div>
            </div>1
        </CardContent>
    </Card>
);





const MetricChart = ({metric, data, isLoading}: MetricChartProps) => {
    const chartConfig = metricChartConfigurations[metric];


    if (isLoading || !data) {
        return <ChartSkeleton />;
    }


    return (
        <Card >
            <CardHeader>
                <CardTitle>{chartConfig.label}</CardTitle>
                <CardDescription>{chartConfig.label} over the last 24 hours.</CardDescription>
            </CardHeader>
            <CardContent>
                <ChartContainer config={chartConfig} className={"h-[250px] w-full"}>
                    <LineChart
                        accessibilityLayer
                        data={data}
                        margin={{
                            left: 12,
                            right: 12,
                        }}
                    >
                        <CartesianGrid vertical={false} />
                        <XAxis
                            dataKey="timestamp"
                            tickMargin={8}

                        />
                        <YAxis width={30} />
                        <ChartTooltip
                            content={<ChartTooltipContent hideLabel />}
                        />
                        <Line
                            dataKey={metric}
                            type="natural"
                            strokeWidth={2}
                            dot={false}
                        />
                    </LineChart>
                </ChartContainer>
            </CardContent>
        </Card>



    )
};

export default MetricChart;