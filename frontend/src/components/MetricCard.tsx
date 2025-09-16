import React from "react";
import {
    Card,
    CardContent,
    CardHeader,
    CardTitle,
} from "@/components/ui/card"
import {ArrowDown, ArrowUp, Minus} from "lucide-react";

type MetricCardProps = {
    label: string;
    value: number;
    unit: string;
    icon: React.ReactNode;
    change: number | null;
}

const cardStyles = {
    'temperature': {
        color: 'text-orange-500',
    },
    'humidity': {
        color: 'text-blue-500',
    },
    'pressure': {
        color: 'text-purple-500',
    }
}

type CardStyleKey = keyof typeof cardStyles;

const MetricCard = ({ label, value, unit, icon, change}: MetricCardProps) => {
    const cardStyle = cardStyles[label.toLowerCase() as CardStyleKey];

    const getChangeIndicator = () => {
        if (change === null || change === undefined) {
            return null;
        }

        let colorClass = 'text-slate-500';
        let ChangeIcon = <Minus size={16} />;

        if (change > 0) {
            colorClass = 'text-green-500';
            ChangeIcon = <ArrowUp size={16} />;
        }
        if (change < 0) {
            colorClass = 'text-red-500';
            ChangeIcon = <ArrowDown size={16} />;
        }

        return (
            <div className={`flex items-center gap-1 font-semibold ${colorClass}`}>
                {ChangeIcon}
                <span>{Math.abs(change).toFixed(1)}</span>
            </div>
        );
    };

    return (
        <Card>
            <CardHeader>
                <CardTitle>
                    <div className="flex items-center gap-2">
                        <div className={cardStyle.color}>
                            {icon}
                        </div>
                        <p>
                            {label}
                        </p>
                    </div>
                </CardTitle>
            </CardHeader>

            <CardContent>
                <div className="flex justify-between items-baseline mb-4">
                    <p className="text-3xl font-bold">
                        {value === null ? "N/A" : `${value.toFixed(1)} ${unit}`}
                    </p>
                    {getChangeIndicator()}
                </div>
            </CardContent>
        </Card>

    );


}


export default MetricCard;